# LEE
Logic Expression Evaluator (LEE)

Program for the LCS class

LEE 2022 [version 1.1 64-bit]

      ---Commands---
APPLY   [RESOLUTION|DAVIS_PUTNAM]                                       Applies the given algorithm on a clause set
BUILD   [TREE|TABLE|NNF|DNF|CNF|SIMPLIFIEDFORM|CLAUSESET]               Computes or modifies a given object
EXIT                                                                    Ends the program
GET     [STRING|CLAUSES|INTERPETATION|STRONGFORM|TREE|CLAUSESET]        Outputs the given object or variable
HELP                                                                    Outputs this message
PAUSE                                                                   Waits until the ENTER key is pressed
RUN     %path%                                                          Executes a LEE script
SAVE    [STRING|CLAUSES]                                                Saves the given variable for later use
SEP                                                                     Outputs a separator
SET     [VERBOSE_LEVEL|ALLOW_RELAXED_SYNTAX|ALLOW_TABLE_EVAL] %value%   Sets a value for the given system variable
SET     [STRING|CLAUSES|INTERPRETATION] *access_point* / %string%       Sets a value for the given variables
SET     [OUT_FILE] %path%                                               Allows passing a file for output

     ---Variables & Objects---

STRING                 Holds the expression used for building binary logic trees
INTERPRETATION         Holds the string used for interpreting binary logic trees
CLAUSES                Holds the string used for building clause sets

TREE                   Structure for binary logic trees
TABLE                  Structure for logic table
CLAUSESET              Structure for clause sets

VERBOSE_LEVEL          System variable for output verbosity (0 - 4)
ALLOW_RELAXED_SYNTAX   Allows the use of relaxed synthax for logic expressions
ALLOW_TABLE_EVAL       Allows the program to use logic tables when buiding normal form
OUT_FILE               Holds the location of the output file

      ---Access Points---

$STRONGFORM$   Retrieves the strong synthax variant of an expression 
$TREE$         Retrieves an expression from a binary logic tree
$SAVE$         Retrieves a previously saved expression or clause
$CLAUSESET$    Retrieves a logic expression in CNF from a clause set

