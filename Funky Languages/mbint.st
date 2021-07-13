(*MiniBasic Interpreter, using Smalltalk
  Written by Kyle Serio. All professor provided code has been removed*)

"Abstract base class for expressions"
MiniBasic subclass: Expr
[ 
   nan := 0.0 / 0.0.
   printOn: file [^ file << self prefix << ')']
   value [^ self]
].

"Expr subclass to handle numeric values"
Expr subclass: NumExpr 
[
   |value|

   NumExpr class >> new: val [^ super new init: val]
   init: val [value := val + 0.0d0]
   printOn: file [^ file << self prefix << value << ')']
   value [^ value + 0.0]
].

"Expr subclass to handle variable creation and access"
Expr subclass: VarExpr 
[
   |variable|

   varDict := Dictionary from: 
   {
      #e   -> 1.0 exp.
      #eof -> 0.0.
      #nan -> (0.0 / 0.0).
      #pi  -> -1.0 arcCos.
   }.
   VarExpr class >> new: var [^ super new init: var]
   init: var [variable := var]
   printOn: file [^ file << self prefix << variable << ')']
   put: val [varDict at: variable put: val]
   value [^ varDict at: variable ifAbsent: [0.0]]
].

"Expr subclas to handle unary operators"
Expr subclass: UnopExpr 
[
   |oper opnd|

   UnopExpr class >> new: op with: opn 
   [^ super new init: op init: opn.]

   init: op init: opn 
   [oper := op. opnd := opn.]

   printOn: file 
   [^ file << self prefix << oper << ' ' << opnd << ')'.]

   value [^ opnd value perform: oper.]
].

"Expr subclas to handle binary operators"
Expr subclass: BinopExpr 
[
   |oper opnd1 opnd2|

   BinopExpr class >> new: op with: opn1 with: opn2
   [^ super new init: op with: opn1 with: opn2.]

   init: op with: opn1 with: opn2 
   [oper := op. opnd1 := opn1. opnd2 := opn2.]

   printOn: file 
   [
      ^ file << self prefix << oper << ' ' << opnd1
             << ' ' << opnd2 << ')'.
   ]

   value [^ opnd1 value perform: oper with: opnd2 value]
].

"Expr subclas to handle array creation and access"
Expr subclass: ArrayExpr 
[
   |arrayName|

   arrayDict := Dictionary new.
   ArrayExpr class >> new: var [^ super new init: var]
   init: var [arrayName := var]
   get [^ arrayName]
   put: val [arrayDict at: arrayName put: val]
   printOn: file [^ file << self prefix << arrayName << ')']
   value[
      ArrayExpr.arrayDict at: arrayName ifAbsent: [stdout << 'No Such Array!' << nl. ObjectMemory quit: 1.].
      ^ ArrayExpr.arrayDict at: arrayName]
].

"Extends the previously defined Expr to properly handle each Expr type"
Expr extend 
[
   Expr class >> parse: expr 
   [
      expr isNumber ifTrue: [^ NumExpr new: expr].
      expr isSymbol ifTrue: [^ VarExpr new: expr].
      expr isArray ifTrue: [
         expr size = 2 ifTrue: [
            ^ UnopExpr new: (expr at: 1)
                       with: (Expr parse: (expr at: 2)).
         ].
         expr size = 3 ifTrue: [
            ((expr at: 1) = #aget) | ((expr at: 1) = #aput)
            ifTrue: [
               ^ BinopExpr new: (expr at: 1)
                           with: (ArrayExpr new: (expr at: 2))
                           with: (Expr parse: (expr at: 3)).
            ]
            ifFalse: [
               ^ BinopExpr new: (expr at: 1)
                           with: (Expr parse: (expr at: 2))
                           with: (Expr parse: (expr at: 3)).
            ]
         ].
         self assertionFailed: 'Expr>>parse: isArray wrong size'.
      ].
      self assertionFailed: 'Expr>>parse: expr wrong type'.
   ]
].

"Abstract subclass to handle statements"
MiniBasic subclass: Stmt
[ 
   nan := 0.0/0.0.
   stmtNr := 1.
   Stmt class >> getStmtNr [^ stmtNr]
   Stmt class >> incrStmtNr [stmtNr := stmtNr + 1]
   Stmt class >> setStmtNr: newNum [stmtNr := newNum]
   labelDict := Dictionary new.
   put: val [labelDict at: stmtNr put: val]
   printOn: file [^ file << self prefix << ')']
   interp [^ self]
].

"Stmt subclass to handle array creation"
Stmt subclass: DimStmt 
[
   |name size|

   DimStmt class >> new: stmt [^ super new init: stmt]

   init: stmt 
   [
      name := ArrayExpr new: (stmt at: 2).
      size := Expr parse: (stmt at: 3).
   ]

   interp[name put: size value]

   printOn: file [^ file << self prefix << name << ' ' << size << ')']
].

"Stmt subclass to handle assigning values to variables"
Stmt subclass: LetStmt 
[
   |name expr|

   LetStmt class >> new: stmt [^ super new init: stmt]

   init: stmt 
   [
      name := Expr parse: (stmt at: 2).
      expr := Expr parse: (stmt at: 3).
   ]

   interp[name put: expr value]

   printOn: file [^ file << self prefix << name << ' ' << expr << ')']
].

"Stmt subclass to handle program execution control; jumping to labels"
Stmt subclass: GotoStmt 
[
   |label|

   GotoStmt class >> new: stmt [^ super new init: stmt]
   num := 0.
   testNum := 0.
   init: stmt [label := stmt at: 2.]

   interp[
      Stmt.labelDict at: label ifAbsent: [stdout << 'No Such Label!' << nl. ObjectMemory quit: 1.].
      Stmt setStmtNr: (Stmt.labelDict at: label)]

   printOn: file [^ file << self prefix << label << ')']
].

"Stmt subclass to handle if statements"
Stmt subclass: IfStmt 
[
   |expr label|

   IfStmt class >> new: stmt [^ super new init: stmt]

   init: stmt 
   [
      expr := Expr parse: (stmt at: 2).
      label := stmt at: 3.
   ]

   interp[expr value ifTrue: [Stmt setStmtNr: (Stmt.labelDict at: label)]]
     
   printOn: file [^ file << self prefix << expr << ' ' << label << ')']
].

"Stmt subclass to handle user input"
Stmt subclass: InputStmt 
[
   |inputVars|

   stdinNumbers := NumberStream new.
   InputStmt class >> new: stmt [^ super new init: stmt]

   init: stmt 
   [
      |args|
      args := stmt at: 2.
      inputVars := Array new: args size.
      1 to: args size do: [:index|
         inputVars at: index put: (Expr parse: (args at: index)).
      ]
   ]

   printOn: file [^ file << self prefix << inputVars << ')']

   interp 
   [
      inputVars do: [:var|
         |value|
         value := stdinNumbers nextNumber.
         (value isNaN) ifTrue: [ObjectMemory quit: 0].
         var put: value.
      ].
   ]
].