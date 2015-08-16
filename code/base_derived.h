#ifndef THIS_INCLUDE_FILE
#define THIS_INCLUDE_FILE

template <class Data>
class Base
{
public:
	int doImpl(Data& someData)
	{
		return (int)someData; 
	}
};

int doImple()
{
	return 3;
}

template <class Data>
class Derived : public Base<Data>
{
public:
	int callDoImpl(Data& someData)
	{
		return doImpl();
	}
};

#endif
