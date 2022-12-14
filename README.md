# HAW PR Lab 3

These are possible solutions to the PSET of lab 3.

I divided them into two separate versions:

1. Simple
1. Insane (Open at your own risk)

## Version explanation

Each version has a different level of complexitiy. Some elements are not included in all versions.

### STATUS

![SIMPEL](https://img.shields.io/badge/SIMPLE-WORK%20IN%20PROGRESS-yellow?style=for-the-badge)

![INSANE](https://img.shields.io/badge/INSANE-WORKS-success?style=for-the-badge)

## Simple

Check out the video tutorial series: [YouTube](https://www.youtube.com/playlist?list=PLdcKp-rQ6drIDnrY41R3ttXm7ehK7vOvu)

work in progress

## Insane

The insane version includes a fully designed UI (user interface) with animated number draft, matrix visualization, and total game results.

It also allows for automated (random) input for user numbers as well as an automated game mode, in which the program repeatedly plays round after round until it gets "6 correct" or is manually interrupted.

<!-- ![Console Screenshot of main](./img/insane_example.png) -->
<div>
  <img src="./img/insane_example.png" alt="Console Screenshot of main" width="200"/>
  <img src="./img/insane_example_default.gif" alt="Console Screenshot of main" width="200"/>
  <img src="./img/insane_example_matrix.gif" alt="Console Screenshot of main" width="200"/>
</div>

### How to use

First, you need to build the binary with `make` or alternatively directly with `clang` or `gcc`. No external local headerfiles are used.

```sh
$ make main
cc     main.c   -o main
```

Then run the executable.

```sh
$ ./main
...
```

#### Flags

You can modify the execution of the program by providing CLI arguments.

```sh
$ ./main -r    # random auto-input for user numbers

$ ./main -m    # print animated matrix (7x7 table)

$ ./main -f    # force win

$ ./main -a    # automated game (still requires manual user input)
```

You can also combine these flags into a single flag. The order is irrelevant.

```sh
$ ./main -rm    # random auto-input & print matrix
```

To completely run the game automated use the `-ra` flag.
```sh
$ ./main -ra    # random auto-input & automated game
```
