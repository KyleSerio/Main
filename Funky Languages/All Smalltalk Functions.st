//prints all integers from 1 to 1000
1 to: 1000 do: [:i | stdout << i << Character nl].

//sum members of an array, no higher order functions
sum1 := [:a|          
   |s| s := 0.
   a do: [:n| s := s + n].
   s.
].

//sum using todo
sum2 := [:a|
   |s| s := 0.
   1 to: a size do: [:i| s := s + (a at: i)].
   s.
].

//sum extending array
Array extend [
   sum [
      |sum|
      sum := 0.
      self do: [:n| sum := sum + n].
      ^ sum.
   ]
].

//extends array to reverse the elements of an array
Array extend [
   reverse [
      1 to: self size // 2 do: [:i|
         |j t|
         j := self size - i + 1.
         t := self at: i.
         self at: i put: (self at: j).
         self at: j put: t.
      ]
   ]
].

//prints numbers 1 to 1000
1 to: 1000 do: [:i | stdout << i << Character nl].

//find the given value and return index of match - same as below?
Array extend [
   find: value [
      1 to: self size do: [:i |
         (self at: i) = value ifTrue: [^i].
      ].
      ^nil
   ]
].

//extend the array with a func that will find the first matching element
Array extend [
   findpos: value [
      1 to: self size do: [:index|
         (self at: index) = value ifTrue: [^ index]
      ].
      ^ nil
   ]
].

//finds first key/value pair that matches the given predicate, and returns the matching value
Array extend [
   findpred: pred [
      self do: [:a|
         (pred value: (a at: 1)) ifTrue: [^ a at: 2].
      ].
      ^ nil.
   ]
].

//fold left 
Array extend [
   fold: block unit: id [
      |result|
      result := id.
      self do: [:item | result := block value: result value: item.].
      ^ result.
   ]
].


// Define a class Counter which
//    maintains an instance variable.  Members are add:
//    which increments the counter;  value, which
//    retrieves the value;  init, which resets it to
//    zero;  and new.

Object subclass: Counter [
   |count|
   Counter class >> new [ ^ super new init ]
   init [ count := 0 ]
   add: amt [ count := count + amt ]
   value [ ^ count ]
]

//define class animal with method new which sets the name field,
instance method setName to change the name and name to return the name
Object subclass: Animal [
   |name|
   Animal class >> new: name_ [ ^ super new setName: name_ ]
   setName: name_ [ name := name_ ]
   name [ ^ name ]
].

//sublclasses of animal, with methods to return the noise they make
Animal subclass: Cat [
   noise [ ^ 'meow' ]
].
Animal subclass: Dog [
   noise [ ^ 'woof' ]
].

//class stack - implement stack and some functions
Object subclass: Stack [
    |array top|
    Stack class >> new: size [
        ˆ super new init: size
    ]
    init: size [
        top := 0.
        array := Array new: size.
    ]

pop [
   |result|
   result := array at: top.
   top := top - 1.
   ^ result.
]
push: item [
   top := top + 1.
   array at: top put: item
]
isempty [
   ^ top = 0.
]

//gcd - find greatest common denominator of two given numbers
Integer extend [
   gcd: other [
      |x y|
      x := self.
      y := other.
      [x ~= y] whileTrue: [
         x > y ifTrue: [x := x - y]
               ifFalse: [y := y - x]
      ].
      ^ x
   ]
].

//inner product
Array extend [
   innerprod: other [
      self size = other size
      ifFalse: [
         ^ self error: 'innerprod different sizes'
      ]
      ifTrue: [
         |sum|
         sum := 0.
         1 to: self size do: [:i|
            sum := sum + (self at: i) * (other at: i)
         ].
         ^ sum
      ]
   ]
]

//extends class number to add a factorial function
Number extend [
   fac [
      |f n|
      n := self.
      (n < 0) ifTrue: [^ nil]
              ifFalse: [
                 f := 1.
                 [n > 1] whileTrue: [ f := f * n. n := n - 1 ].
                 ^ f
      ]
   ]
].

//Extend class Array with the keyword message contains: which accepts a block with one argument as
//a predicate. Return true if any element of the array, when sent to the predicate via the value: message return
//true
Array extend [
   contains: predicate [
      1 to: self size do: [:i|
         (predicate value: (self at: i)) ifTrue: [^ true].
      ].
      ^ false
   ]
]

//print indeces and contents of an array
1 to: a size do: [:i|
   stdout << i << ': ' << (a at: i) << Character nl.
].

//determine the parity of a number and assign an associated string
n \\ 2 = 0 ifTrue: [parity := 'even']
           ifFalse: [parity := 'odd'].