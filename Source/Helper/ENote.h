#pragma once

#include <string>
#include <functional>
#include <unordered_map>

namespace Elixir
{
	//Elixir Engine Notification System
	//A variable oriented non-template Observer-pattern-like class
	//To manage communication between different classes
	class ENote
	{
	public:
		static ENote& GetInstance();
		~ENote();

		//Delete methods that are unnacceptable (no copy, no assign)
		ENote(ENote const&) = delete;
		void operator=(ENote const&) = delete;

		template<typename T>
		T Notify(std::string note);

		template<typename T>
		void AddNote(std::string note, std::function<T()> noteFunc);

		template<typename T>
		void DeleteNote(std::string);

	private:
		ENote() {};

	private:
		std::unordered_map<std::string, std::function<void()>> m_voidNotes;
		std::unordered_map<std::string, std::function<float()>> m_floatNotes;
		std::unordered_map<std::string, std::function<int()>> m_intNotes;
		std::unordered_map<std::string, std::function<bool()>> m_boolNotes;
		std::unordered_map<std::string, std::function<std::string()>> m_stringNotes;
	};

	
}