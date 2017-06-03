#include "ENote.h"

Elixir::ENote & Elixir::ENote::GetInstance()
{
	static ENote instance;
	return instance;
}

Elixir::ENote::~ENote()
{
}

//Notify methods-------------------------------------------------------

template<>
int Elixir::ENote::Notify<int>(std::string note)
{
	if (m_intNotes.count(note) == 0)
		return 0;
		
	auto func = m_intNotes.find(note)->second;
	if(func)
		return func();

	return 0;
}

template<>
void Elixir::ENote::Notify<void>(std::string note)
{
	if (m_voidNotes.count(note) == 0)
		return;

	auto func = m_voidNotes.find(note)->second;
	if (func)
		func();
}

template<>
float Elixir::ENote::Notify<float>(std::string note)
{
	if (m_floatNotes.count(note) == 0)
		return 0;

	auto func = m_floatNotes.find(note)->second;
	if (func)
		return func();

	return 0;
}

template<>
bool Elixir::ENote::Notify<bool>(std::string note)
{
	if (m_boolNotes.count(note) == 0)
		return false;

	auto func = m_boolNotes.find(note)->second;
	if (func)
		return func();

	return false;
}

template<>
std::string Elixir::ENote::Notify<std::string>(std::string note)
{
	if (m_stringNotes.count(note) == 0)
		return "";

	auto func = m_stringNotes.find(note)->second;
	if (func)
		return func();

	return "";
}

//AddNote methods-------------------------------------------------------

template<>
void Elixir::ENote::AddNote<void>(std::string note, std::function<void()> noteFunc)
{
	m_voidNotes[note] = noteFunc;
}

template<>
void Elixir::ENote::AddNote<float>(std::string note, std::function<float()> noteFunc)
{
	m_floatNotes[note] = noteFunc;
}

template<>
void Elixir::ENote::AddNote<int>(std::string note, std::function<int()> noteFunc)
{
	m_intNotes[note] = noteFunc;
}

template<>
void Elixir::ENote::AddNote<bool>(std::string note, std::function<bool()> noteFunc)
{
	m_boolNotes[note] = noteFunc;
}

template<>
void Elixir::ENote::AddNote<std::string>(std::string note, std::function<std::string()> noteFunc)
{
	m_stringNotes[note] = noteFunc;
}

//DeleteNote methods-------------------------------------------------------

template<>
void Elixir::ENote::DeleteNote<void>(std::string note)
{
	//if exists
	if (m_voidNotes.count(note) != 0)
	{
		m_voidNotes.erase(note);
	}
}

template<>
void Elixir::ENote::DeleteNote<int>(std::string note)
{
	//if exists
	if (m_intNotes.count(note) != 0)
	{
		m_intNotes.erase(note);
	}
}

template<>
void Elixir::ENote::DeleteNote<float>(std::string note)
{
	//if exists
	if (m_floatNotes.count(note) != 0)
	{
		m_floatNotes.erase(note);
	}
}

template<>
void Elixir::ENote::DeleteNote<bool>(std::string note)
{
	//if exists
	if (m_boolNotes.count(note) != 0)
	{
		m_boolNotes.erase(note);
	}
}

template<>
void Elixir::ENote::DeleteNote<std::string>(std::string note)
{
	//if exists
	if (m_stringNotes.count(note) != 0)
	{
		m_stringNotes.erase(note);
	}
}