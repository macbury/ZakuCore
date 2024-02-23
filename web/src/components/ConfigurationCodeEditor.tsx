import React from "react";
import Ajv, { JTDSchemaType } from "ajv/dist/jtd"

// https://ajv.js.org/guide/schema-language.html#json-type-definition
type Entity = {
  id: string,
  name: string,
  kind: "SIMPLE_LED",
  pin: "D0" | "D1"| "D2" | "D3" | "D4" | "D5" | "D6" | "D7" | "D8" | "D9" | "D10",
  brightness?: number,
  color?: {
    r: number,
    g: number,
    b: number,
  },
}

type EntitiesList = Entity[]

// Load schema from remote server?
const schema: JTDSchemaType<EntitiesList> = {
  elements: {
    properties: {
      id: { type: "string" },
      name: { type: "string" },
      kind: { enum: ["SIMPLE_LED"] }, // look at "Discriminator" schemas https://jsontypedef.com/docs/jtd-in-5-minutes/#elements-schemas
      pin: { enum: ["D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "D10"] },
    },

    optionalProperties: {
      brightness: { type: "uint8" },
      color: {
        properties: {
          r: { type: "uint8" },
          g: { type: "uint8" },
          b: { type: "uint8" }
        }
      }
    }
  },
}

const ajv = new Ajv()
const parse = ajv.compileParser(schema)

export interface IConfigurationCodeEditorProps {
  value: string,
  style: any,
  onChange(value : string)
}

async function errorMarker(view: EditorView) : Promise<Diagnostic[]> {
  const data = parse(view.state.toJSON().doc);
  if (data === undefined) {
    const { from, to } = view.lineBlockAt(parse.position || 0);
    const message = parse.message || "";
    return [
      { from, to, message, severity: "error" }
    ];
  }

  return [];
}

export default function ConfigurationCodeEditor({ value, onChange, ...props} : IConfigurationCodeEditorProps) {
  return (
    <textarea value={value} onChange={(e) => onChange(e.target.value)} {...props}></textarea>
  )
  // return (
  //   <textarea
  //     value={value}
  //     theme={darcula}
  //     extensions={[json(), linter(errorMarker), lintGutter()]}
  //     onChange={(value, viewUpdate) => {
  //       onChange(value)
  //     }}
  //     {...props}
  //   />
  // )
}
