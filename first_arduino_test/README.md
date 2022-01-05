# My first IDF project for ESP32-C3

## menuconfig

```bash
docker run --rm -it -v $(pwd):/project -w /project --user $(id -u):$(id -g) espressif/idf:release-v4.4 idf.py menuconfig
```

## savedefconfig

Works in `master`, will work in releases after `4.4`.

```bash
docker run --rm -it -v $(pwd):/project -w /project --user $(id -u):$(id -g) espressif/idf:release-v4.4 idf.py save-defconfig
```

## Build

```bash
docker run --rm -it -v $(pwd):/project -w /project --user $(id -u):$(id -g) --tmpfs /.ccache espressif/idf:release-v4.4 idf.py build
```

## Run

```bash
docker run --rm -it -v $(pwd):/project -w /project --user $(id -u):$(id -g) --group-add 20 --tmpfs /.ccache --device /dev/ttyUSB3:/dev/ttyUSB0 espressif/idf:release-v4.4 idf.py -p /dev/ttyUSB0 flash monitor
```
