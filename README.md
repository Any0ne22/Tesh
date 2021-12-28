# Tesh

A simple POSIX Shell made for a school project at TELECOM Nancy.

## Building

Make sure you have `gcc`, `libreadline` and `make` installed, then run :

```bash
make tesh
```

## Usage

You can run **Tesh** in interactive mode with `./tesh` (or with `./tesh -r` to use `libreadline`).

To run a script, you can do

```bash
./tesh script.sh
```

or

```bash
cat script.sh | ./tesh
```

or

```bash
./tesh < script.sh
```

## Authors

- Amélie FERSTLER (amelie.ferstler@telecomnancy.eu)
- Tom GOUVILLE (tom.gouville@telecomnancy.eu)
