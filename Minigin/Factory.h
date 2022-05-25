#pragma once
#include <unordered_map>
#include <functional>
#include <iostream>

#include "Singleton.h"



namespace dae
{
	// https://stackoverflow.com/questions/43653962/is-that-possible-to-know-all-the-name-of-derived-classes
	template <typename Base>
	class Factory : public Singleton<Factory<Base>>
	{
	public:
		template<class Derived>
		void RegisterClassToFactory()
		{
			m_ClassCreators.insert({ typeid(Derived).name(), []()-> Base* {return new Derived(); } });
		}

		Base* Create(const std::string& derivedName)
		{
			//assert(m_ClassCreators.find(derivedName) != m_ClassCreators.end());

			const auto it = m_ClassCreators.find(derivedName);
			if (it == m_ClassCreators.end()) return nullptr;
			return (it->second)();

			// This would be an o(1) operation but does not have safety
			//return m_ClassCreators[derivedName]();
		}

		void PrintRegisteredClasses()
		{
			for (const auto& classRegister : m_ClassCreators)
			{
				std::cout << classRegister.first << std::endl;
			}
		}

	private:
		friend class Singleton<Factory<Base>>;
		Factory<Base>() = default;


		std::unordered_map < std::string, std::function<Base*()>> m_ClassCreators;
	};

	template <typename Base, typename Derived>
	class Creator
	{
	public:
		Creator()
		{
			Factory<Base>::GetInstance().RegisterClassToFactory<Derived>();
		}
	};
}

