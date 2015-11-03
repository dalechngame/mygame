/********************************************************************
	created:	2014/08/22
	created:	22:8:2014   15:27
	file ext:	hpp
	author:		lic
	purpose:	as template.
*********************************************************************/
#ifndef __CONFIGDATATEMPLATE__HPP__
#define __CONFIGDATATEMPLATE__HPP__
#include <string> 
#include <map> 
#include <vector>

template<typename T>
class ConfigDataTemplate
{
public:
	static void						 Release		(void);
	static T&						 Tem			(std::string key);
	static std::vector<T*>&			 Lis			(std::string contextKey);
	static std::map<std::string,T*>& Dic			(void);
protected:
	static std::map<std::string,T*> mapdata;
	static std::vector<T*>			arr;
	static bool						isInstance;
	static std::string				lastLoadLanguage;
};

template<typename T>
std::vector<T*> ConfigDataTemplate<T>::arr;

template<typename T>
bool ConfigDataTemplate<T>::isInstance = false;

template<typename T>
std::map<std::string,T*> ConfigDataTemplate<T>::mapdata;

template<typename T>
std::string ConfigDataTemplate<T>::lastLoadLanguage = "None";
													 
template<typename T>
T& ConfigDataTemplate<T>::Tem( std::string key )
{
	T::InitData();
	T* t = mapdata[key];
	return (*t);
}

template<typename T>
std::vector<T*>& ConfigDataTemplate<T>::Lis( std::string contextKey )
{
	T::InitData();
	arr.clear();
	typename std::map<std::string,T*>::iterator _it;
	const char *  								src;
	const char *								dst;
	std::string									tempStr;
	int											ret;
	
	if(contextKey.c_str()[contextKey.length()-1] != ':')
		contextKey += ":";

	_it = mapdata.begin();
	while (_it != mapdata.end())
	{
		tempStr  = _it->first;
		tempStr += ":";

 		src = tempStr.c_str();
 		dst = contextKey.c_str();
 
 		ret = 1;
 	
 		while(*dst &&! (ret = *(unsigned char *)src - *(unsigned char *)dst))
 		{
 			++src, ++dst;
 		}
 
 		if(ret == 0)
 		{
 			arr.push_back(_it->second);
 		}
		++_it; 
	} 

	return arr;
}

template<typename T>
std::map<std::string,T*>& ConfigDataTemplate<T>::Dic( void )
{
	T::InitData();
	return mapdata;
}

template<typename T>
void ConfigDataTemplate<T>::Release(void)
{
	if(mapdata.size())
	{
		typename std::map<std::string,T*>::iterator _it;
		_it = mapdata.begin();
		while (_it != mapdata.end())
		{
			delete _it->second;
			++_it;				  
		}
		mapdata.clear();
		arr.clear();
	}
}
#endif