import React, { useEffect, useState } from "react";
import { useParams } from "react-router-dom";
import { Text } from "@arwes/core";
import Actions from "../components/Actions";
import CenteredContent from "../components/CenteredContent";
import LinkButton from "../components/LinkButton";
import Window from "../components/Window";

const MAX_FAILED_TIMES = 10;
const SECONDS_LEFT = 60;

export default function WaitForRebootPage() {
  const [failedTimes, setFailedTimes] = useState(0);
  const [waitSeconds, setSecondsLeft] = useState(SECONDS_LEFT);
  const { hostname } = useParams();
  const somethingWrong = waitSeconds <= 0;

  useEffect(() => {
    const endpoint = `http://${hostname}.local/api/ping`;

    const handler = setInterval(async () => {
      try {
        await fetch(endpoint);
        window.location.href = `http://${hostname}.local/`;
        clearTimeout(handler);
      } catch (e) {
        console.log("Try again...");
        setFailedTimes((times) => times + 1);
      }
    }, 10000);

    const timer = setInterval(() => {
      setSecondsLeft((t) => {
        if (t > 0) {
          return t - 1
        } else {
          return t
        }
      });
    }, 1000);

    return () => {
      clearTimeout(handler);
      clearTimeout(timer);
    }
  }, [hostname])

  return (
    <CenteredContent>
      <Window title="Rebooting..." activate={!somethingWrong}>
        <br/>
        <Text>Waiting up to</Text> <Text>{waitSeconds}</Text> <Text>seconds for reboot to complete</Text> <Text as="p">and <a href={`http://${hostname}.local`}>http://{hostname}.local</a> become available</Text>
      </Window>

      <Window activate={somethingWrong}>
        <Text as="p">Setup failed for some reason. If you still see ZakuCore wifi network then connect to it and try again</Text>
      </Window>

      <Actions>
        <LinkButton to="/wifi/setup" activate={somethingWrong}>
          Try Again
        </LinkButton>
      </Actions>
    </CenteredContent>
  )
}
