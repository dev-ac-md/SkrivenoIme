/*****************************************************************************/
/*	File:	kHash.hpp
/*	Desc:	Hash tables
/*	Author:	Ruslan Shestopalyuk
/*	Date:	19.12.2002
/*****************************************************************************/

#ifndef __KHASH_HPP__
#define __KHASH_HPP__

const int c_DefTableSize	= 17389; // 2000s prime number
const int c_DefMinPoolSize	= 16384;

#define NO_ELEMENT		0xFFFFFFFF 
/*****************************************************************************/
/*	Class:	Hash
/*	Desc:	pooled hash for generic elements, with self-adjustable pool
/*				size
/*	Remark:	EType should have functions
/*				unsigned int	hash	() const;
/*				bool			equal	( const EType& el );
/*				void			copy	( const EType& el );
/*			defined. 
/*****************************************************************************/
template	<	
			class	EType, 
			int		tableSize,	//= c_DefTableSize, 
			int		minPoolSize //= c_DefMinPoolSize
			>
class Hash
{
	//  internal class Entry
	class Entry : public EType
	{
	public:
		Entry() : next((Entry*)0xFFFFFFFF), EType() {}
		bool		isFree() const			{ return (next == (Entry*)0xFFFFFFFF);	}
		void		setFree()				{ next = 0xFFFFFFFF;					}
		Entry*		next;
	}; // class Entry

	Entry*			pool;				//  pool for elements
	int				poolSize;			//  current pool size
	int				nEntriesUsed;		//  current used number of pool entries
	Entry*			table[tableSize];	//  hash table
public:
	//  ctor
			Hash() 
			{
				pool		= new Entry[minPoolSize];
				poolSize	= minPoolSize;
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );
			}
	//  dtor
			virtual ~Hash()
			{
				delete []pool;
			}
	int		numElem() const { return nEntriesUsed; }
	EType&	elem( int idx ) { assert( idx >= 0 && idx < nEntriesUsed ); return pool[idx]; }
	//  reset all nafiq
	void	reset()
			{
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );	
			}
	//  adds element to hash table, returns its pool index, if exists
	int		add		( const EType& el )
			{
				//  get hash index
				unsigned int h = el.hash() % tableSize;
				Entry* entry = table[h];
				//  null pointer in the hash table => allocate
				if (!entry)
				{
					Entry& added	= allocEntry();
					added.next		= 0;
					table[h]		= &added;
					added.copy( el );
					return (&added) - pool;	
				}
				if (entry->isFree())
				//  ha! free hash table entry!
				{
					entry->copy( el );
					entry->next = 0;
					return entry - pool;
				}
				int idx = locateEntry( *entry, el );
				//  check if we have already that element
				if (idx != 0xFFFFFFFF) return idx;
				//  no element - add it to the head of linked list
				Entry& added	= allocEntry();
				added.next		= table[h];
				table[h]		= &added;
				added.copy( el );
				return (&added) - pool;
			}
	int		find	( const EType& el )
			{
				//  get hash index
				unsigned int h = el.hash() % tableSize;
				if (table[h] == 0 ||
					table[h]->isFree()) return NO_ELEMENT;
				//  ok, one or bunch of entries with same hash value
				return locateEntry( *(table[h]), el );
			}
	void	dump	( FILE* fp )
			{
				fprintf( fp, "Dumping hash table: %X\n", this );
				for (int i = 0; i < nEntriesUsed; i++)
				{
					pool[i].dump( fp );
				}
			}
protected:
	//  enlarges pool with given number of entries
	void	growPool( int numEntriesToGrow )
			{
				poolSize += numEntriesToGrow;
				Entry* npool = new Entry[poolSize];
				memcpy( npool, pool, nEntriesUsed * sizeof( Entry ) );
				//  redirect table references
				for (int i = 0; i < tableSize; i++)
				{
					if (table[i]) table[i] = npool + (table[i] - pool);
				}
				for (int i = 0; i < nEntriesUsed; i++)
				{
					if (npool[i].next)
					{
						npool[i].next = npool + (npool[i].next - pool);
					}
				}
				delete []pool;
				pool = npool;
			}
	//  allocate hash entry from the pool
	Entry&	allocEntry()
			{
				if (nEntriesUsed == poolSize)
				//  no more entries, grow pool
				{
					growPool( poolSize / 2 );
				}
				return pool[nEntriesUsed++];
			}
	//  finds entry for element in the linked list
	//  returns 0xFFFFFFFF if not found
	int		locateEntry( const Entry& first, const EType& el )
			{
				Entry* cur = const_cast<Entry*>( &first );
				if (cur->isFree()) return 0xFFFFFFFF;
				do
				{
					if (cur->equal( el )) return cur - pool;
					cur = cur->next;
				}
				while (cur && !(cur->isFree()));
				return 0xFFFFFFFF;
			}
}; // class Hash

/*****************************************************************************/
/*	Class:	StaticHash
/*	Desc:	pooled hash for generic elements, with static pool size
/*****************************************************************************/
template	<
	class	EType,
	class	KType,
	int		tableSize, //= c_DefTableSize,
	int		maxEntries // = c_DefPoolSize
	>
class StaticHash
{
	//  internal class Entry
	class Entry : public EType
	{
	public:
		Entry() : next((Entry*)0xFFFFFFFF), EType() {}
		bool		isFree() const			{ return (next == (Entry*)0xFFFFFFFF);	}
		void		setFree()				{ next = 0xFFFFFFFF;					}
		
		KType		key;
		Entry*		next;
	}; // class Entry

	Entry			pool[maxEntries];	//  pool for elements
	int				nEntriesUsed;		//  current used number of pool entries
	
	Entry*			table[tableSize];	//  hash table
public:
	//  ctor
			StaticHash() 
			{
				reset();
			}
	
	int		numElem() const { return nEntriesUsed; }
	EType&	elem( int idx ) { assert( idx >= 0 && idx < nEntriesUsed ); return pool[idx]; }
	int		nextInChain( int idx )
	{
		assert( idx >= 0 && idx < nEntriesUsed );
		Entry& entry = pool[idx];
		if (entry.next) return entry.next - &pool;
		return NO_ELEMENT;
	}
	//  reset all nafiq
	void	reset()
			{
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );	
			}
	//  adds element to hash table, returns its pool index, if exists
	int		add		( const KType& key, const EType& el )
			{
				//  get hash index
				unsigned int h = key.hash() % tableSize;
				Entry* entry = table[h];
				//  null pointer in the hash table => allocate
				if (!entry)
				{
					Entry& added	= allocEntry();
					added.next		= 0;
					table[h]		= &added;
					added.copy( el );
					added.key		= key; 
					return (&added) - pool;	
				}
				if (entry->isFree())
				//  ha! free hash table entry!
				{
					entry->copy( el );
					entry->key = key;
					entry->next = 0;
					return entry - pool;
				}
				int idx = locateEntry( *entry, key );
				//  check if we have already that element
				if (idx != 0xFFFFFFFF) return idx;
				//  no element - add it to the head of linked list
				Entry& added	= allocEntry();
				added.next		= table[h];
				table[h]		= &added;
				added.copy( el );
				added.key		= key; 
				return (&added) - pool;
			}
	int		find	( const KType& key )
			{
				//  get hash index
				unsigned int h = key.hash() % tableSize;
				if (table[h] == 0 ||
					table[h]->isFree()) return NO_ELEMENT;
				//  ok, one or bunch of entries with same hash value
				return locateEntry( *(table[h]), key );
			}
	void	dump	( FILE* fp )
			{
				fprintf( fp, "Dumping hash table: %X\n", this );
				for (int i = 0; i < nEntriesUsed; i++)
				{
					pool[i].dump( fp );
				}
			}
protected:
	//  allocate hash entry from the pool
	Entry&	allocEntry()
			{
				assert( nEntriesUsed < maxEntries );
				return pool[nEntriesUsed++];
			}
	//  finds entry for element in the linked list
	//  returns NO_ELEMENT if not found
	int		locateEntry( const Entry& first, const KType& key )
			{
				Entry* cur = const_cast<Entry*>( &first );
				if (cur->isFree()) return NO_ELEMENT;
				do
				{
					if (cur->key == key) return cur - pool;
					cur = cur->next;
				}
				while (cur && !(cur->isFree()));
				return NO_ELEMENT;
			}
}; // class StaticHash

/*****************************************************************************/
/*	Class:	PointerHash
/*	Desc:	hash for generic elements' pointers, with self-adjustable pool size
/*	Remark:	EType should have functions
/*				unsigned int	hash	() const;
/*				bool			equal	( const EType& el );
/*				void			copy	( const EType& el );
/*			defined. 
/*****************************************************************************/
template	<	
			class	EType, 
			class	KType,
			int		tableSize	= c_DefTableSize, 
			int		minPoolSize = c_DefMinPoolSize
			>
class PointerHash
{
	//  internal class Entry
	class Entry 
	{
	public:
		Entry() : next((Entry*)0xFFFFFFFF) {}
		bool		isFree() const			{ return (next == (Entry*)0xFFFFFFFF);	}
		void		setFree()				{ next = 0xFFFFFFFF;					}
		
		EType*		el;
		KType		key;

		Entry*		next;
	}; // class Entry

	Entry*			pool;				//  pool for elements
	int				poolSize;			//  current pool size
	int				nEntriesUsed;		//  current used number of pool entries
	Entry*			table[tableSize];	//  hash table
public:
	//  ctor
			PointerHash() 
			{
				pool		= new Entry[minPoolSize];
				poolSize	= minPoolSize;
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );
			}
	//  dtor
			virtual ~PointerHash()
			{
				delete []pool;
			}
	int		numElem() const { return nEntriesUsed; }
	EType*	elem( int idx ) 
	{ 
		assert( idx >= 0 && idx < nEntriesUsed ); 
		return pool[idx].el; 
	}
	Entry& entry( int idx ) 
	{ 
		assert( idx >= 0 && idx < nEntriesUsed ); 
		return pool[idx]; 
	}
	EType*	c_elem( int idx ) const 
		{ 
			assert( idx >= 0 && idx < nEntriesUsed ); 
			return pool[idx].el; 
		}
	
	//  reset all nafiq
	void	reset()
			{
				nEntriesUsed= 0;
				memset( table, 0, tableSize * sizeof( Entry* ) );	
			}
	//  adds element to hash table, returns its pool index, if exists
	int		add		( const KType& key, EType* el )
			{
				//  get hash index
				unsigned int h = key.hash() % tableSize;
				Entry* entry = table[h];
				//  null pointer in the hash table => allocate
				if (!entry)
				{
					Entry& added	= allocEntry();
					added.next		= 0;
					table[h]		= &added;
					added.key		= key;
					added.el		= el;
					return (&added) - pool;	
				}
				if (entry->isFree())
				{
					entry->el		= el;
					entry->next		= 0;
					entry->key		= key;
					return entry - pool;
				}
				int idx = locateEntry( *entry, key );
				//  check if we have already that element
				if (idx != 0xFFFFFFFF) return idx;
				//  no element - add it to the head of linked list
				Entry& added	= allocEntry();
				added.next		= table[h];
				table[h]		= &added;
				added.el		= el;
				added.key		= key;
				return (&added) - pool;
			}
	int		find	( const KType& key )
			{
				//  get hash index
				unsigned int h = key.hash() % tableSize;
				if (table[h] == 0 ||
					table[h]->isFree()) return NO_ELEMENT;
				//  ok, one or bunch of entries with same hash value
				return locateEntry( *(table[h]), key );
			}
	void	dump	( FILE* fp )
			{
				fprintf( fp, "Dumping hash table: %X\n", this );
				for (int i = 0; i < nEntriesUsed; i++)
				{
					pool[i].dump( fp );
				}
			}
protected:
	//  enlarges pool with given number of entries
	void	growPool( int numEntriesToGrow )
			{
				poolSize += numEntriesToGrow;
				Entry* npool = new Entry[poolSize];
				memcpy( npool, pool, nEntriesUsed * sizeof( Entry ) );
				//  redirect table references
				for (int i = 0; i < tableSize; i++)
				{
					if (table[i]) table[i] = npool + (table[i] - pool);
				}
				for (int i = 0; i < nEntriesUsed; i++)
				{
					if (npool[i].next)
					{
						npool[i].next = npool + (npool[i].next - pool);
					}
				}
				delete []pool;
				pool = npool;
			}
	//  allocate hash entry from the pool
	Entry&	allocEntry()
			{
				if (nEntriesUsed == poolSize)
				//  no more entries, grow pool
				{
					growPool( poolSize / 2 );
				}
				return pool[nEntriesUsed++];
			}
	//  finds entry for element in the linked list
	//  returns 0xFFFFFFFF if not found
	int		locateEntry( const Entry& first, const KType& key )
			{
				Entry* cur = const_cast<Entry*>( &first );
				if (cur->isFree()) return 0xFFFFFFFF;
				do
				{
					if (cur->key == key) return cur - pool;
					cur = cur->next;
				}
				while (cur && !(cur->isFree()));
				return 0xFFFFFFFF;
			}
}; // class PointerHash

//  some default hash functions
inline unsigned int hash0( BYTE* s, int nBytes )
{
	unsigned int h = 0;
	for (int i = 0; i < nBytes; i++)
	{
		h = (h * 729 + (*s) * 37) ^ (h >> 1);
		s++;
	}
	return h;
} // hash

#endif // __KHASH_HPP__
