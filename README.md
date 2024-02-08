# IOU
This is a fun little project in C made out of pure boredom. \
It is somewhat useful though and you may use it to manage your debts! \
And it even saves everything into a file!

## Dependencies
You will need `git` for cloning the repo and `gcc` and `make` for building the program. \
If your distribution does not come with any of those, install them:
```
# apt install git gcc make
```
```
# pacman -S git gcc make
```

## Installation and setup
1. Clone the GitHub repo
```
$ git clone https://github.com/jantar1154/iou
```
2. Cd into the `iou` directory
```
$ cd iou
```
3. Build the program
```
$ make
```
4. Open `config.ini` file and edit the contents to your liking
- *debtfilelocation*: a path where debt file will be saved. Make sure all folders exist!\
For example:
```
debtfilelocation:/home/chucknorris/.iou/debt.dat
```
**More available settings may be added in the future**\
5. Run the program
```
$ ./iou
```
> **NOTE:** 
> Alternatively, you can pass an argument which would be path to file other than default `./debt.dat` file! \
> For example: `$ ./iou /tmp/my_temp_debt.dat`!
> Second argument can be your another config file.

## Usage
You can refer to this chapter or write `help` into the prompt.

_**Available commands are:**_
- **help**:  shows available commands
- **list**:  list your debts
- **count**: prints number of debts you have
- **add**:   adds new entry
- **edit**:  edits existing entry
- **exit**:  exits the program
