# Led group
- multiple pins one led


# Light setup
  - show pinout like https://pinout.xyz/pinout/pin7_gpio4
  - clicking on pin shows right sidebar or new page(mobile)
    - light setup page
      - quick test
        - switch on and off the pin if light is not setup
      - enabled
      - select light type
        - affect ui below
      - light name

# State

- store state in state.db
- add persist and restore to entity
- use json structure
  {
    "states": [
      { "id": "test", on: true, b: 255, color: { r: 0, g: 0, b: 0 } },
      { "id": "multiled", "lights": [{ id: "test", on: true }] }
    ]
  }
- entity manager has store, restore
  - load state.db
  - iterate over,
  - find entity by id

# Redirect to domain
- generate uid in /id
- server this id using ping
- if current hostname != status hostname
  - ping status hostname
    - if id equals current one
      - redirect to new hostname

# Automation mode
  - cron
    - control using ntp time setup
  - mqtt
    - control using commands over mqtt

# create light
  - button to add new light
  - select pin number
    - list of available pins
  - select light type:
    - rgb strip
    - single led
  - light name
  - save
    - redirect to light edit page
# edit light
  - depending on type show different form

# edit simple light
  - change name
  - change icon
  - change color
  - test light button



# Admin panel
- reset factory settings


# Update improvement
- on update write current version in spiff /current-version
- create endpoint /api/update-completed
  - if file /current-version exists compare with current version
    - return json with
      - prev: prev version
      - current ver
    - remove the file
    - on ui show message that update is completed

# Light editor

- replace board wit switch to switch all?
- Load config file using endpoint
- Show ui for modify this json
- Use config to update that json
- Reload entities enpoint hit
- use websocket for listening
- MessageMultiples
  - multiplex from websocket and mqtt
  - send
  - receive


# Captive portal upgrade

- open browser page instead current one


# Led calculator
- you can input led V and am and then calculate the resistor

## Github page

- led calculator
- esphome upload interface
  - select version
    - get release list
    - select newest
  - connect to serial
  - upload
