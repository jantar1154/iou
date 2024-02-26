# IOU
This is a fun little project in C made out of pure boredom. \
It is somewhat useful though and you may use it to manage your debts! \
And it even saves everything into a file!

PRs and issues are welcome!

## 1. Dependencies
You will need `git` for cloning the repo and `gcc` and `make` for building *iou* and `ncurses`. \
Your linux distribution might not come with any of those, so install them:

**1.1. Apt**:
```
# apt install git gcc make ncurses
```
**1.2. Pacman**:
```
# pacman -S git gcc make ncurses
```

## 2. Installation and setup
1. `cd` into the directory you wish to contain source code

2. Clone this GitHub repo
```
$ git clone https://github.com/jantar1154/iou
```
3. Cd into the `iou` directory
```
$ cd iou
```
4. Build the program
```
$ make
```
5. Open `config.ini` file and edit the contents to your liking
- *debtfilelocation*: a path where debt file will be saved. Make sure all folders exist!\
For example:
```
debtfilelocation:/home/chucknorris/.iou/debt.dat
```
**More available settings may be added in the future** \
6. Run the program
```
$ ./iou
```
> **NOTE:** \
> Alternatively, you can pass an argument which would be path to file other than default `./debt.dat` file! \
> For example: `$ ./iou /tmp/my_temp_debt.dat`! \
> Second argument can be your another config file.

## 3. Usage
You can refer to this chapter or write `help` into the prompt.

### 3.1 Available commands: 
- **help**:  shows available commands
- **list**:  list your debts
- **query**: same as `list`, but you can give a query
- **count**: prints the number of debts you have
- **add**:   adds a new entry
- **edit**:  edits an existing entry
- **exit**:  exits the program

### 3.2 Please note:
My command line input does not currently support arrow keys for looking back.

Also write commands exactly as they are, no whitespaces and such.

## 4. Roadmap
1. Able to edit configuration file from the program itself
2. Add **git** support (not sure if I want to do it)
3. Drastically improve saving/query system *or* completely switch to **sqlite**
4. Switch to **ncurses** or improve the TUI and commands system

Your ideas are welcome in *issues*. You can alternatively open a pull request!