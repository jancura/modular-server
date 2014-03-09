//hlavickovy subor HandlerRegister.h, sablona pre polia handlerov

#ifndef MY_HANDLER_REGISTER
#define MY_HANDLER_REGISTER

#define SIZE_HANDLER 10
#define SIZE_NAME    20

template <class Handler>
class HandlerRegister
{
public:
	HandlerRegister<Handler>();
	~HandlerRegister<Handler>();

	bool	  Register(Handler fHandler, char* pName=NULL);
	Handler GetHandler(const char* pName);
	index  	IndexOfName(const char * pName) ;
	
	inline int Size() { return m_iSizeHandlers; }

	Handler& operator[](index i)  { return m_ArrayHandlers[i]; }
	const Handler& operator[](index i) const { return m_ArrayHandlers[i]; }

protected:
private:
	bool Resize();

private:
	int			m_iSizeHandlers;
	int			m_iSizeArray;
	Handler  *	m_ArrayHandlers;
	char	**	m_ArrayNameHandlers;
};

template<class Handler>
inline HandlerRegister<Handler>::HandlerRegister()
{
	m_iSizeHandlers = 0;
	m_iSizeArray	= SIZE_HANDLER;
	m_ArrayHandlers     = (Handler*)malloc(SIZE_HANDLER*sizeof(Handler));
	m_ArrayNameHandlers = (char**)malloc(SIZE_HANDLER*sizeof(char*));
	for (int i=0; i<SIZE_HANDLER; ++i) {
		m_ArrayNameHandlers[i]= NULL;
	}
}


template<class Handler>
inline HandlerRegister<Handler>::~HandlerRegister()
{
	for (index i=0; i<m_iSizeHandlers; ++i) {
		if (m_ArrayNameHandlers[i]) delete [] m_ArrayNameHandlers[i];
	}
	free(m_ArrayNameHandlers);
	free(m_ArrayHandlers);
}

template<class Handler>
bool HandlerRegister<Handler>::Register(Handler fHandler, char* psName/* =NULL */)
{
	if (m_iSizeHandlers >= m_iSizeArray ) {
		if (!Resize()) return false;
	}
	m_ArrayHandlers[m_iSizeHandlers]=fHandler;
	if (psName) {
		m_ArrayNameHandlers[m_iSizeHandlers] = new char [strlen(psName)+1];
		strcpy(m_ArrayNameHandlers[m_iSizeHandlers], psName);
	}
	++m_iSizeHandlers;
	return true;
}

template<class Handler>
bool HandlerRegister<Handler>::Resize()
{
	Handler* pH=(Handler*)realloc(m_ArrayHandlers, (m_iSizeArray+SIZE_HANDLER)*sizeof(Handler));
	char **  pN=(char**)realloc(m_ArrayNameHandlers, (m_iSizeArray+SIZE_HANDLER)*sizeof(char*));
	if (pN || pH) return false;
	m_ArrayHandlers=pH; m_ArrayNameHandlers=pN;
	for (int i=m_iSizeArray; i<m_iSizeArray+SIZE_HANDLER; ++i) {
		m_ArrayNameHandlers[i]=NULL;
	}
	m_iSizeArray+=SIZE_HANDLER;
	return true;
}

template<class Handler>
index HandlerRegister<Handler>::IndexOfName(const char * psName)
{
	for (index i=0; i<m_iSizeHandlers; ++i) {
		if (m_ArrayNameHandlers[i] && !strcmp(m_ArrayNameHandlers[i],psName)) 
			return i;
	}
	return -1;
}

template <class Handler>
Handler HandlerRegister<Handler>::GetHandler(const char * psName)
{
	index i=IndexOfName(psName);
	if (i>-1) {
		return m_ArrayHandlers[i];
	}
	return NULL;
}

#endif