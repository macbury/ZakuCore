import React, { useState } from "react";
import styled from "@emotion/styled";
import { useNavigate } from "react-router-dom";
import { useAppDispatch } from "../../stores/store";
import flashSlice from "../../stores/slices/flash";
import { useForm } from "react-hook-form";
import { LoadingBars, Text } from "@arwes/core";
import Tabs from "../../components/admin/Tabs";
import Window from "../../components/Window";
import CenteredContent from "../../components/CenteredContent";
import Actions from "../../components/Actions";
import ActionButton from "../../components/ActionButton";
import { hashMD5 } from "../../helpers/md5";
import { useGetStatusQuery, useRestartMutation } from "../../stores";

const Loader = styled(LoadingBars)`
  z-index: 1000;
  backdrop-filter: blur(12px);
`;

export interface IFirmwareUploadState {
  loading: boolean,
  progress: number
}

export type UpdateResponse = {
  success: boolean,
  errors: string[]
}

export default function UpdateFirmwarePage() {
  const navigate = useNavigate();
  const [uploading, setUploading] = useState<IFirmwareUploadState>({ loading: false, progress: 0 })
  const dispatch = useAppDispatch();
  const [restart] = useRestartMutation();
  const {
    data: status,
    isSuccess
  } = useGetStatusQuery();
  const {
    register,
    handleSubmit,
  } = useForm();

  const uploadFirmware = (formData) => {
    return new Promise<UpdateResponse>(async (resolve, reject) => {
      const request = new XMLHttpRequest();
      request.upload.addEventListener('progress', (e) => {
        setUploading(({ progress, ...data }) => ({
          ...data,
          progress: Math.trunc((e.loaded / e.total) * 100)
        }));
      });
      request.withCredentials = true;
      request.addEventListener('load', () => {
        // request.response will hold the response from the server
        if (request.status === 200) {
          resolve(JSON.parse(request.responseText) as UpdateResponse)
        } else if (request.status !== 500) {
          reject(request.statusText)
        } else {
          reject(request.responseText);
        }
      });

      request.open('post', '/api/firmware/update');
      request.send(formData);
    });
  }

  const validateForm = handleSubmit(async (data) => {
    setUploading({ loading: true, progress: 0 });

    const firmware = data.firmware[0];
    const formData = new FormData();
    const hash = await hashMD5(firmware);

    formData.append('checksum', hash);
    formData.append('firmware', firmware, 'firmware');

    if (!confirm("Are you sure you wan't to upload firmware with hash:\n" + hash)) {
      setUploading({ loading: false, progress: 0 });
      return;
    }

    try {
      const { success, errors } = await uploadFirmware(formData);
      if (success) {
        dispatch(flashSlice.actions.show({ message: "Updated firmware!", type: "success" }))
        await restart(null)
        navigate(`/reboot/${status?.wifi.hostname}`, { replace: true });
      } else {
        setUploading({ loading: false, progress: 0 });
        dispatch(flashSlice.actions.show({ type: "error", message: errors.join(", ") }))
      }
    } catch (e : any) {
      dispatch(flashSlice.actions.show({ type: "error", message: e }))
      console.error(e);
    }
  })

  return (
    <React.Fragment>
      <CenteredContent>
        <Tabs />
        <form onSubmit={validateForm}>
          <Window title="Firmware Upgrade" activate={isSuccess}>
            <Text as="p">
              The latest firmware version includes updates from the previous version.
              Configuration parameters will keep their settings during the firmware update process.
              Update will take up to 2 minutes. <b>Please do not leave this page and don't power off device</b>.
            </Text>

            <Text as="p">
              <b>Current Version:</b> {status?.firmware?.name}
            </Text>

            <label htmlFor="firmware"><Text>New Firmware File:</Text></label>
            <input disabled={uploading.loading} type="file" {...register('firmware', { required: true })} />
          </Window>
          <Actions>
            <ActionButton disabled={uploading.loading} active  palette="error" onClick={uploadFirmware}>
              Update firmware now
            </ActionButton>
          </Actions>
        </form>
      </CenteredContent>
      {uploading.loading && <Loader
        size={2}
        speed={4}
        determinate={uploading.progress < 99.0}
        progress={uploading.progress}
        full
      />}
    </React.Fragment>
  );
}
