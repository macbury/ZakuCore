from os.path import join, realpath
from jinja2 import Environment, FileSystemLoader, select_autoescape
import json
import os

def generate_pins(config, default_state = "LOW"):
  env = Environment(
    loader=FileSystemLoader(os.path.abspath("./scripts/template/")),
    autoescape=select_autoescape()
  )

  json_file = open(os.path.abspath('./config/pinouts/'+config+'.json'))
  pins = json.load(json_file)
  template = env.get_template("pins_board.h.jinja")

  f = open(os.path.abspath("./include/pins_board.h"), "w")
  f.write(template.render(pins=pins, default_state = default_state))
  f.close()
