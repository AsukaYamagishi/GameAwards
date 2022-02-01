#pragma once

//安全にdeleteするやつ
template <class T>
inline void safe_delete(T*& p)
{
	delete p;
	p = nullptr;
}
