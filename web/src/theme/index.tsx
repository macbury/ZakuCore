import React from 'react';
import { ArwesThemeProvider, StylesBaseline } from '@arwes/core';
import { BleepsProvider } from '@arwes/sounds';
import { AnimatorGeneralProvider } from '@arwes/animation';
import SOUND_ASSEMBLE_URL from './sounds/assemble.mp3'; //TODO: move to separate component
import SOUND_TYPE_URL from './sounds/type.mp3';
import SOUND_CLICK_URL from './sounds/click.mp3';

const FONT_FAMILY_ROOT = '"Titillium Web", sans-serif'; //TODO: try to inline include?
const FONT_FAMILY_CODE = '"Source Code Pro", monospace';

function fixMp3DataUrl(url) {
  //esbuld has problem with detecting mime types for mp3...
  return url.replace('data:application/octet-stream;', 'data:audio/mpeg;')
}

const animatorGeneral = { duration: { enter: 200, exit: 200 } };
const audioSettings = { common: { volume: 0.5 } };
const playersSettings = {
  assemble: { src: [fixMp3DataUrl(SOUND_ASSEMBLE_URL)], loop: true, preload: true },
  type: { src: [fixMp3DataUrl(SOUND_TYPE_URL)], loop: true, preload: true },
  click: { src: [fixMp3DataUrl(SOUND_CLICK_URL)], preload: true }
};

const bleepsSettings = {
  assemble: { player: 'assemble' },
  type: { player: 'type' },
  click: { player: 'click' }
};

const style = {
  'html, body': { fontFamily: FONT_FAMILY_ROOT },
  'code, pre': { fontFamily: FONT_FAMILY_CODE }
}

const themeSettings = {};

export default function Theme({ children }) {
  return (
    <ArwesThemeProvider themeSettings={themeSettings}>
      <StylesBaseline styles={style} />
      <BleepsProvider audioSettings={audioSettings} playersSettings={playersSettings} bleepsSettings={bleepsSettings}>
        <AnimatorGeneralProvider animator={animatorGeneral}>
          {children}
        </AnimatorGeneralProvider>
      </BleepsProvider>
    </ArwesThemeProvider>
  )
}
