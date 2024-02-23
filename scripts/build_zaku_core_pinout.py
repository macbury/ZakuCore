Import('env')
from generate_pins import generate_pins

generate_pins("zaku_core_v1", default_state="LOW")
