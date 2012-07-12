# Gorgone
Turning mutability to stone.

C++ implementations of RRBVector ([paper here](http://infoscience.epfl.ch/record/169879/files/RMTrees.pdf)) and PersistentVector.  

## Dependencies:

Boost 1.48

## Known issues
 
RRBVector does not yet include fast concatentation.


main.cpp provides an example:  

Creates a PersistentVector of Persistent vectors of long called v2.
Iteratively inserts new vectors into v2, each one copy of the previous with a new data added.
Iteratively calls pop() until v2 is empty.
Writes the ellapsed time between operations;

Repeats this procedure with two kind of Vectors in order to compare times:
TranisentVectorCopy<T>. A transient vector that uses an std::vector<T>.
TransientVectorSPtr<T>. A transient vector that uses an std::vector<shared_ptr <T> >

Repeats the procedures again with a block of 10 chars.


