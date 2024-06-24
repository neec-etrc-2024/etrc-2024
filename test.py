import runner
import etrobo_python
from device import Device

g_device = Device()
g_line_tracer = runner.LineTrace(g_device, runner.LineTrace.Mode.RIGHT, 50, 0.2)
is_started = False

g_left: etrobo_python.Motor = None
g_right: etrobo_python.Motor = None
g_color: etrobo_python.ColorSensor = None


def main():
    etrobo = etrobo_python.etrobo.ETRobo(backend="simulator")
    etrobo.add_hub("hub")
    etrobo.add_device("left", device_type=etrobo_python.Motor, port="C")
    etrobo.add_device("right", device_type=etrobo_python.Motor, port="B")
    etrobo.add_device("color", device_type=etrobo_python.ColorSensor, port="2")
    # etrobo.add_handler(test_run)
    etrobo.add_handler(test_dev)
    etrobo.dispatch()


def test_run(
    hub: etrobo_python.Hub,
    left: etrobo_python.Motor,
    right: etrobo_python.Motor,
    color: etrobo_python.ColorSensor,
):
    global g_device
    global g_line_tracer
    global is_started

    g_device.set_device(left, right, color)

    if not is_started:
        if hub.is_left_button_pressed():
            left.reset_count()
            right.reset_count()
            is_started = True
        else:
            return

    g_line_tracer.run()


def test_dev(
    hub: etrobo_python.Hub,
    left: etrobo_python.Motor,
    right: etrobo_python.Motor,
    color: etrobo_python.ColorSensor,
):
    global g_right
    global g_left
    global g_color
    global is_started

    if not is_started:
        g_left = left
        g_right = right
        g_color = color
        is_started = True
        return

    print(f"left:{g_left is left}")
    print(f"right:{g_right is right}")
    print(f"color:{g_color is color}")

    g_left.set_power(100)


if __name__ == "__main__":
    main()
