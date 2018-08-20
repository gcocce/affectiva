# Intro

This repo has an example of :) [affectiva](https://www.affectiva.com/) library usage for Raspberry Pi. This library allows you to recognize human emotion in a video. The version used in this example is the version intended for raspbian system.

## Requirements

You need opencv already installed and a webcam attached to the Raspberry Pi.

## Files
There are two important files in this repo:

* src/prueba.c
    -which contains the usage example in cpp.
* compilar.sh
    -which allows to compile the source code. You may need to modify regarding on which directories you install affectiva library.

## Run

Once you compile the example:

```sh
$ ./prueba
```

It will output evry data affectiva library produces each time it detect a face on the webcam.
