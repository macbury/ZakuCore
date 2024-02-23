import React, { useState, useRef, useCallback, useMemo, useEffect } from "react";
import styled from "@emotion/styled";
import throttle from "lodash/throttle";
import { useBleeps } from "@arwes/sounds";

const Container = styled.div`
  padding: 0px 12px 12px;
  width: auto;
  touch-action: pan-y;
`;

const SliderWrapper = styled.div`
  width: 100%;
  height: 40px;
  border-radius: 10px;
  background: rgba(5, 198, 193, 0.2);
`

const Slider = styled.div`
  position: relative;
  width: 100%;
  height: 100%;
  border-radius: 10px;
  transform: translateZ(0px);
  cursor: move;
  overflow: hidden;

  user-select: none;
  -webkit-user-drag: none;
  -webkit-tap-highlight-color: rgba(0, 0, 0, 0);
`

const TrackBar = styled.div`
  position: absolute;
  top: 0px;
  left: 0px;
  width: 100%;
  height: 100%;
  transition: transform 180ms linear 0s, opacity 200ms ease-in-out 0s;
  border-radius: 0 10px 10px 0;

  transform: translate3d(-95%, 0, 0);
  background: var(--track-color, rgba(255, 255, 255, 0.7)); // color of track

  &::after {
    top: 0px;
    bottom: 0px;
    right: 5px;
    height: 50%;
    width: 5px;

    display: block;
    content: "";
    position: absolute;
    margin: auto;
    border-radius: 10px;
    background-color: var(--accent-color, #fff);
  }
`;

export interface IBrightnessFeature {
  value: number;
  enabled?: boolean;
  max: number;
  onValueChange(value : number);
}

export default function BrightnessFeature({ value, enabled, onValueChange, max, ...props } : IBrightnessFeature) {
  const bleeps = useBleeps();
  const ref = useRef<HTMLDivElement>(null);
  const [drag, setDrag] = useState(false);
  const [progress, setProgress] = useState(0.0);

  useEffect(() => {
    setProgress(value / max)
  }, [value, max])

  const calculateProgress = useCallback((ev) => {
    const container = ref.current
    if (!container) {
      return 0
    }
    const rect = container.getBoundingClientRect();

    const x = ev.pageX - rect.x;
    const percent = x / rect.width;

    if (percent > 0.95) {
      return 1.0;
    } else if (percent <= 0.05) {
      return 0.05;
    }

    return percent;
  }, [ref])

  const throttledValueChange = useMemo(() => throttle(onValueChange, 100), [onValueChange])

  useEffect(() => {
    if (drag) {
      const value = Math.round(progress * max);
      throttledValueChange(value);
    }
  }, [progress, max, throttledValueChange, drag])

  const onDragStart = useCallback((ev : React.MouseEvent<HTMLDivElement, MouseEvent>) => {
    ev.preventDefault();

    const progress = calculateProgress(ev);
    setDrag(true);
    setProgress(progress);
  }, [setDrag, calculateProgress, setProgress])

  const onTouchStart = useCallback((ev : React.TouchEvent<HTMLDivElement>) => {
    ev.preventDefault();

    setDrag(true);
    const progress = calculateProgress(ev.touches[0]);
    setProgress(progress);
  }, [setProgress])

  const onDragMove = useCallback((ev : React.MouseEvent<HTMLDivElement, MouseEvent>) => {
    ev.preventDefault();
    if (drag) {
      const progress = calculateProgress(ev);
      setProgress(progress);
    }
  }, [setProgress, drag])

  const onTouchMove = useCallback((ev : React.TouchEvent<HTMLDivElement>) => {
    if (drag) {
      const progress = calculateProgress(ev.touches[0]);
      setProgress(progress);
    }
  }, [setProgress, drag])

  const onTouchEnd = useCallback((ev : React.TouchEvent<HTMLDivElement>) => {
    ev.preventDefault();
    setDrag(false);
    bleeps.click.play();
  }, [setDrag])

  const onDragEnd = useCallback((ev : React.MouseEvent<HTMLDivElement, MouseEvent>) => {
    ev.preventDefault();

    const progress = calculateProgress(ev);
    setProgress(progress);
    setDrag(false);
    bleeps.click.play();
  }, [setDrag, setProgress])

  const transform = useMemo(() => {
    let x = 100 - Math.round(progress * 100);
    return `translate3d(-${x}%, 0, 0)`;
  }, [progress, enabled])

  return (
    <Container {...props} onClick={ev => ev.preventDefault()}>
      <SliderWrapper>
        <Slider
          ref={ref}
          onMouseDown={onDragStart}
          onMouseMove={onDragMove}
          onMouseUp={onDragEnd}
          onTouchStart={onTouchStart}
          onTouchMove={onTouchMove}
          onTouchEnd={onTouchEnd}>
          <TrackBar style={{ transform, opacity: enabled ? 1.0 : 0.3 }} />
        </Slider>
      </SliderWrapper>
    </Container>
  )
}
