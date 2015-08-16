
template <class Data>
class Base
{
public:
	int doImpl(Data someData)
	{
		return (int)someData; 
	}
};

int doImpl(int a)
{
	return 4;
}

template <class Data>
class Derived : public Base<Data>
{
public:
	int callDoImpl(Data someData)
	{
		return this->doImpl(someData);
	}
};


int main()
{
	Derived<int> a;
	return a.callDoImpl(4);
}
