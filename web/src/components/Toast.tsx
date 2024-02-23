import React from "react";
import { Text, Blockquote } from "@arwes/core";

export interface IToastProps {
  children: any
  palette?: string | null
}

export default function Toast({ children, ...props} : IToastProps) {
  return (
    <Blockquote {...props}>
      <Text>
        {children}
      </Text>
    </Blockquote>
  )
}
