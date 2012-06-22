/*
 * RRBVector.h
 *
 *      Original author: ignacio on github
 */

#ifndef RRBVECTOR_H_
#define RRBVECTOR_H_
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

template <typename T>
class RRBVector
{
private:

	struct Node;
	typedef boost::shared_ptr <Node> node_ptr;
	struct Node
	{
		virtual bool 				   isFull    () const = 0;
		virtual unsigned int   level     () const = 0;
		virtual unsigned int   lastIndex () const = 0;
		virtual unsigned int   size      () const = 0;
		virtual const node_ptr pop_back  () const = 0;
		virtual const node_ptr push_back (const T &value)     const = 0;
		virtual const T &      get       (unsigned int index) const = 0;
		virtual ~Node() {}
	};

	struct LeafBranch : public Node
	{
		unsigned int _size;
		T *_data;

		unsigned int size   ()    const { return _size; }
		bool 				 isFull ()    const { return _size == 32;	}
		unsigned int level  ()    const { return 0; }
		unsigned int lastIndex () const { return _size; }
		const T & get (unsigned int index) const {	return _data[index];	}

		LeafBranch (const T &content) :
			_size (1),
			_data (new T[1])
			{	_data[0] = content; }

		LeafBranch (const T data[], unsigned int data_size, unsigned int new_size) :
      _size (new_size),
      _data (new T[new_size])
			{ for (unsigned int i = 0 ; i < data_size && i < new_size ; i++)
				   _data[i] = data[i];
			}

		LeafBranch (const T data[], unsigned int data_size, const T &to_append) :
			_size (data_size+1),
			_data (new T[data_size+1])
			{ for (unsigned int i = 0 ; i < data_size ; i++)
					_data[i] = data[i];
				data[data_size] = to_append;
			}

		LeafBranch (const LeafBranch &other) :
			_size (other._size),
			_data (new T[other._size])
			{ for (unsigned int i = 0; i < other._size ; i++)
					_data[i] = other._data[i];
			}

		LeafBranch & operator = (const LeafBranch &other)
			{ if (_data)
					delete[] _data;
				_size = other._size;
				_data = new T[other._size];
				for (unsigned int i = 0 ; i < other._size ; i++)
					_data[i] = other._data[i];
				return *this;
			}

		virtual ~LeafBranch() {	delete [] _data;	}

		const node_ptr push_back (const T &value) const
			{ node_ptr newnode = boost::make_shared <LeafBranch> (_data, _size, _size+1);
				static_cast<LeafBranch *> (newnode . get ()) -> _data[_size] = value;
				return newnode;
		  }

		const node_ptr pop_back () const
			{ if (_size == 1)	return node_ptr ();
				else 						return boost::make_shared <LeafBranch> (_data, _size-1, _size-1);
		  }
	};

	struct MidBranch : public Node
	{
		unsigned int _size;
		unsigned int _level;
		unsigned int *_indexes;
		node_ptr *_data;

		unsigned int size   () 	  const { return _size; }
		bool         isFull ()    const { return _size == 32 && _data[31] -> isFull (); }	//order of && eval!
		unsigned int level  () 	  const { return _level; }
		unsigned int lastIndex () const { return _indexes[_size-1]; }

		MidBranch(unsigned int sz, unsigned int lvl) : 
			_size  (sz),
			_level (lvl),
			_indexes (new unsigned int[_size]),
			_data 	 (new boost::shared_ptr<Node>[_size])
			{}

		MidBranch (const node_ptr &new_child) :
			_size    (1),
			_level   (new_child -> level () + 5),
			_indexes (new unsigned int[_size]),
			_data    (new node_ptr[_size])
			{ _data[0]    = new_child;
				_indexes[0] = new_child -> lastIndex ();
			}

		MidBranch (const node_ptr data[],
							 const unsigned int indexes[],
							 unsigned int data_length, unsigned int new_size, unsigned int lvl) :
		  _size    (new_size),
		  _level   (lvl),
			_indexes (new unsigned int[new_size]),
			_data    (new node_ptr[new_size])
			{ for (unsigned int i = 0; i < data_length && i < new_size ; i++)
				{ _data[i]    = data[i];
					_indexes[i] = indexes[i];
				}
			}

		MidBranch (const MidBranch &other) :
			_size    (other._size),
			_level   (other._level),
			_indexes (new unsigned int[size]),
			_data    (new node_ptr[size])
			{ for (unsigned int i = 0 ; i < other.size () ; i++)
					{ _data[i]    = other._data[i];
					  _indexes[i] = other._indexes[i];
				  }
		  }

		virtual MidBranch & operator = (const MidBranch &other)
			{ for (unsigned int i = 0 ; i < _size ; i++)
					_data[i] . reset ();
				delete [] _data;
				delete [] _indexes;
				_level = other . level ();
				_size  = other . size  ();
				_indexes = new unsigned int[_size];
				_data    = new node_ptr[_size];
				for (unsigned int i = 0 ; i < other.size () ; i++)
					{ _data[i]    = other._data[i];
					  _indexes[i] = other._indexes[i];
				  }
				return *this;
			}

		virtual ~MidBranch ()
			{ for (unsigned int i = 0 ; i < _size ; i++)
					_data[i] . reset();
				delete [] _data;
				delete [] _indexes;
			}

		inline bool fullChild (const node_ptr &n) const
			{ return (n -> level () + 5 == _level) && (n -> isFull ());
			}

		//  new -- has a bug
		// const node_ptr push_back (const T &value) const
		// 	{ node_ptr rightChild = _data[_size - 1];
		// 		if (fullChild (rightChild))
		// 			{ node_ptr result =
		// 					boost::make_shared <MidBranch> (_data, _indexes, _size, _size+1, _level);
		// 				MidBranch *newBranch = static_cast<MidBranch *>(result . get ());
		// 				newBranch -> _data[_size]    = boost::make_shared <LeafBranch> (value);
		// 				newBranch -> _indexes[_size] = _indexes[_size-1] + 1;
		// 				return result;
		// 			}
		// 		else
		// 			{
		// 				//  what did this do?  could there be grandchildren?
		// 				// if (rightChild -> isFull ())
		// 				// 	{ rightChild = boost::make_shared <MidBranch> (rightChild);
		// 				// 		unsigned int prevIndex = (_size == 1) ? 0 : _indexes[_size-2];
		// 				// 		static_cast<MidBranch*>(rightChild . get ()) -> _indexes[0] = _indexes[_size-1] - prevIndex;
		// 				// 	}

		// 				//  Copy in all existing kids except the last one, which will be brand new
		// 				node_ptr result =
		// 					boost::make_shared <MidBranch> (_data, _indexes, _size-1, _size, _level);
		// 				MidBranch *resultPtr = static_cast<MidBranch *> (result . get ());
		// 				resultPtr -> _data[_size-1]    = rightChild -> push_back (value);
		// 				resultPtr -> _indexes[_size-1] = _indexes[_size-1] + 1;
		// 				return result;
		// 			}
		// 	}

		// old
		bool isSubnodeFull(Node *node) const
			{ return (node->level()+5==_level)&&(node->isFull());
			}

		// old
		const node_ptr push_back (const T &value) const
		{
			if (isSubnodeFull(_data[_size-1].get()))
				{
					node_ptr result(new MidBranch(_data,_indexes,_size,_size+1,_level));
					MidBranch *newBranch=static_cast<MidBranch *>(result.get());
					LeafBranch nextNode(value);
					// LeafBranch nextNode(1);
					// nextNode._data[0]=value;
					newBranch->_data[_size]=node_ptr (new LeafBranch(nextNode));
					newBranch->_indexes[_size]=_indexes[_size-1]+1;
					return result;
				}
			else
			{
				node_ptr result(new MidBranch(_data,_indexes,_size-1,_size,_level));
				node_ptr newChild;
				if (_data[_size-1]->isFull())
				{
					///Shift children node
					newChild=node_ptr(new MidBranch(1,_data[_size-1]->level()+5));
					MidBranch *newBranch=static_cast<MidBranch *>(newChild.get());
					newBranch->_data[0]=_data[_size-1];
					unsigned int prevIndex=_size-1==0?0:_indexes[_size-2];
					newBranch->_indexes[0]=_indexes[_size-1]-prevIndex;
				}
				else
				{
					newChild=_data[_size-1];
				}
				MidBranch *resultBranch=static_cast<MidBranch *>(result.get());
				resultBranch->_data[_size-1]=newChild->push_back(value);
				resultBranch->_indexes[_size-1]=_indexes[_size-1]+1;
				return result;
			}
		};

		const node_ptr pop_back () const
			{ node_ptr newChild = _data[_size-1] -> pop_back ();
				if (newChild)
					{ node_ptr result =
							boost::make_shared <MidBranch> (_data, _indexes, _size-1, _size, _level);
						MidBranch *newBranch = static_cast<MidBranch *>(result . get ());
						newBranch -> _data[_size-1]    = newChild;
						newBranch -> _indexes[_size-1] = _indexes[_size-1]-1;
						return result;
					}
				if (_size == 1)
					return node_ptr ();
				else if (_size == 2 && _data[_size-2] -> level () > 0)
					return _data[0];

				return boost::make_shared <MidBranch> (_data, _indexes, _size-1, _size-1, _level);
			}

		const T & get (unsigned int index) const
			{ unsigned int branch;
				for (branch = (index >> _level) & 0x1f; _indexes[branch] < index ; branch++)
					;
				unsigned int nextIndex = (branch == 0) ? 0 : _indexes[branch-1];
				return _data[branch] -> get (index - nextIndex);
			}
	};

	mutable node_ptr _root;

public:

	RRBVector<T> ()	{}

  // todo: write constructor to quickly
  // construct RRBVectors from (e.g.) std::vectors
	RRBVector<T> (const std::vector <T> &v)
	  { //_root = boost::make_shared <LeafBranch> (1);
	  	//newVec._root=_root -> push_back (value);
	  }

	RRBVector<T> push_back (const T &value) const
		{ RRBVector<T> newVec;
			if (!_root)
				newVec._root = boost::make_shared <LeafBranch> (value);
			else
				{ if (_root -> isFull ())
						{ //  Push the existing root down to be a child of a new mid-node
							//  todo: this actually causes 2 midbranches to be created, for no good reason
							node_ptr newRoot = boost::make_shared <MidBranch> (_root);
							newVec._root = newRoot -> push_back (value);
						}
					else
						newVec._root = _root -> push_back (value);
				}
			return newVec;
		}

	RRBVector<T> pop_back () const
		{ RRBVector<T> result;
			if (! _root)
				return result;
			result . _root = _root -> pop_back ();
			return result;
		}

	unsigned int size () const
		{ if (!_root) return 0;
		  return _root -> lastIndex();
  	}

	//  For a bad index, will crash mos def
	const T & get (unsigned int index) const
		{ return _root -> get (index);
		}

  inline const T & operator[] (unsigned int index) const  { return get (index); }
	inline const T & at (unsigned int index) 				 const  { return get (index); }

	bool operator == (const RRBVector &other)  const
	  {	for (unsigned int i = 0; i < size (); i++)
	  		if (! (get (i) == other . get (i)))
				  return false;

			return true;
  	}

};

#endif /* RRBVECTOR_H_ */
