#pragma once

//ˆÀ‘S‚Édelete‚·‚é‚â‚Â
template <class T>
inline void safe_delete(T*& p)
{
	delete p;
	p = nullptr;
}
