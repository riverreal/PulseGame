#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace Elixir
{
	enum EASING_FUNCTION
	{
		ET_LINEAR,
		ET_QUADRANTIC_IN,
		ET_QUADRANTIC_OUT,
		ET_QUADRANTIC_IN_OUT,
		ET_CUBIC_IN,
		ET_CUBIC_OUT,
		ET_CUBIC_IN_OUT,
		ET_QUINT_IN,
		ET_QUINT_OUT,
		ET_QUINT_IN_OUT,
		ET_BACK_IN,
		ET_BACK_OUT,
		ET_BACK_IN_OUT
	};

	template <class T>
	struct tweenData
	{
	public:
		tweenData(int tID)
			:tweenID(tID)
		{}

		inline bool operator==(const tweenData& td2)
		{	
			return this->tweenID == td2.tweenID && this->initialValue == td2.initialValue && this->finalValue == td2.finalValue
				&& this->timeCounter == td2.timeCounter && this->reversed == td2.reversed && this->easing == td2.easing;
		}

		int tweenID;
		bool fromReady;
		bool byReference;
		bool reversed = false;
		T* initialValue;
		T initialCpy;
		T finalValue;
		float duration;
		EASING_FUNCTION easing = ET_LINEAR;
		float timeCounter;
		std::function<void()> callback;
		std::vector<tweenData<T>> endTween;
	};

	template <class T>
	class ETween
	{
	public:
		ETween();
		~ETween();

		ETween From(T* initVal);
		ETween To(T finalVal);
		ETween Time(float sec);
		ETween OnFinish(std::function<void()> endCallback);
		ETween OnFinishChain(ETween<T>* chain);
		ETween Reversed(bool isReversed);
		ETween Easing(EASING_FUNCTION easingType);

		std::vector<tweenData<T>> GetTweens();
		void ReleaseTweens();

		void AddTweens(std::vector<tweenData<T>> tweens);

		void Update(float dt);
		void AddTween(tweenData<T> tween);
	private:
		T Linear(float position, T start, T end);
		T QuadIn(float position, T start, T end);
		T QuadOut(float position, T start, T end);
		T QuadInOut(float position, T start, T end);
		T CubicIn(float position, T start, T end);
		T CubicOut(float position, T start, T end);
		T CubicInOut(float position, T start, T end);
		T QuintIn(float position, T start, T end);
		T QuintOut(float position, T start, T end);
		T QuintInOut(float position, T start, T end);
		T BackIn(float position, T start, T end);
		T BackOut(float position, T start, T end);
		T BackInOut(float position, T start, T end);

	private:
		int m_lastTweenIndex;
		std::vector<tweenData<T>> m_tweenVec;
	};

	template<class T>ETween<T>::ETween()
		:m_lastTweenIndex(-1)
	{}

	template<class T>ETween<T>::~ETween()
	{}

	template<class T> void ETween<T>::ReleaseTweens()
	{
		m_tweenVec.clear();
		m_lastTweenIndex = -1;
	}

	template<class T>ETween<T> ETween<T>::From(T* initVal)
	{
		tweenData<T> tween(m_lastTweenIndex);

		tween.fromReady = true;
		tween.byReference = true;
		tween.initialValue = initVal;
		tween.initialCpy = *initVal;
		tween.duration = 0;
		tween.timeCounter = 0;
		tween.endTween = std::vector<tweenData<T>>();

		m_tweenVec.push_back(tween);

		m_lastTweenIndex++;

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T>ETween<T> ETween<T>::To(T finalVal)
	{
		m_tweenVec[m_lastTweenIndex].finalValue = finalVal;

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T>ETween<T> ETween<T>::Time(float sec)
	{
		m_tweenVec[m_lastTweenIndex].duration = sec;

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T> void ETween<T>::Update(float dt)
	{
		std::vector<tweenData<T>> tweensToAdd;
		std::vector<tweenData<T>> tweensToDelete;

		for (auto &tween : m_tweenVec)
		{
			if (tween.fromReady)
			{
				float timePos = tween.timeCounter / tween.duration;
				//auto value = tween.initialCpy + (tween.timeCounter / tween.duration) * (tween.finalValue - tween.initialCpy);
				T value, start, end;

				if (tween.reversed)
				{
					start = tween.finalValue;
					end = tween.initialCpy;
				}
				else
				{
					start = tween.initialCpy;
					end = tween.finalValue;
				}

				switch (tween.easing)
				{
				case ET_LINEAR:
					value = Linear(timePos, start, end);
					break;
				case ET_QUADRANTIC_IN:
					value = QuadIn(timePos, start, end);
					break;

				case ET_QUADRANTIC_OUT:
					value = QuadOut(timePos, start, end);
					break;

				case ET_QUADRANTIC_IN_OUT:
					value = QuadInOut(timePos, start, end);
					break;

				case ET_CUBIC_IN:
					value = CubicIn(timePos, start, end);
					break;

				case ET_CUBIC_OUT:
					value = CubicOut(timePos, start, end);
					break;

				case ET_CUBIC_IN_OUT:
					value = CubicInOut(timePos, start, end);
					break;

				case ET_QUINT_IN:
					value = QuintIn(timePos, start, end);
					break;

				case ET_QUINT_OUT:
					value = QuintOut(timePos, start, end);
					break;

				case ET_QUINT_IN_OUT:
					value = QuintInOut(timePos, start, end);
					break;

				case ET_BACK_IN:
					value = BackIn(timePos, start, end);
					break;

				case ET_BACK_OUT:
					value = BackOut(timePos, start, end);
					break;

				case ET_BACK_IN_OUT:
					value = BackInOut(timePos, start, end);
					break;

				default:
					value = Linear(timePos, start, end);
					break;
				}

				*tween.initialValue = value;

				if (tween.timeCounter >= tween.duration)
				{
					if (tween.reversed)
						*tween.initialValue = tween.initialCpy;
					else
						*tween.initialValue = tween.finalValue;

					tween.fromReady = false;
					if (tween.callback)
					{
						tween.callback();
					}

					for (auto &callbackTw : tween.endTween)
					{
						tweensToAdd.push_back(callbackTw);
					}

					tweensToDelete.push_back(tween);
				}

				tween.timeCounter += dt;
			}
		}

		for (tweenData<T> &oldTween : tweensToDelete)
		{
			//m_tweenVec.erase(std::remove(m_tweenVec.begin(), m_tweenVec.end(), oldTween), m_tweenVec.end());
			std::vector<tweenData<T>>::iterator position = std::find(m_tweenVec.begin(), m_tweenVec.end(), oldTween);
			if (position != m_tweenVec.end())
			{
				m_tweenVec.erase(position);
				m_lastTweenIndex--;
			}
		}

		for (auto &newTween : tweensToAdd)
		{
			AddTween(newTween);
		}
	}

	template<class T>ETween<T> ETween<T>::OnFinish(std::function<void()> endCallback)
	{
		m_tweenVec[m_lastTweenIndex].callback = endCallback;

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T>ETween<T> ETween<T>::OnFinishChain(ETween<T>* chain)
	{
		m_tweenVec[m_lastTweenIndex].endTween = chain->GetTweens();

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T>ETween<T> ETween<T>::Reversed(bool isReversed)
	{
		m_tweenVec[m_lastTweenIndex].reversed = isReversed;

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T>ETween<T> ETween<T>::Easing(EASING_FUNCTION easingType)
	{
		m_tweenVec[m_lastTweenIndex].easing = easingType;

		ETween<T> ret;
		ret = *this;
		return ret;
	}

	template<class T>std::vector<tweenData<T>> ETween<T>::GetTweens()
	{
		return m_tweenVec;
	}

	template<class T>void ETween<T>::AddTween(tweenData<T> tween)
	{
	
		tween.tweenID = m_lastTweenIndex;
		m_tweenVec.push_back(tween);
		m_lastTweenIndex++;
	}

	template<class T>void ETween<T>::AddTweens(std::vector<tweenData<T>> tweens)
	{
		for (auto &tween : tweens)
		{
			AddTween(tween);
		}
	}

	template<class T>T ETween<T>::Linear(float position, T start, T end)
	{
		return static_cast<T>((end - start) * position + start);
	}

	template<class T>T ETween<T>::QuadIn(float position, T start, T end)
	{
		return static_cast<T>((end - start) * position * position + start);
	}

	template<class T>T ETween<T>::QuadOut(float position, T start, T end)
	{
		return static_cast<T>(((end - start)* -1) * position * (position - 2) + start);
	}

	template<class T>T ETween<T>::QuadInOut(float position, T start, T end)
	{
		position *= 2;
		if (position < 1)
			return static_cast<T>(((end - start) / 2) * position * position + start);

		--position;
		return static_cast<T>((((end - start) * 1) / 2)* (position * (position - 2) - 1) + start);
	}

	template<class T>T ETween<T>::CubicIn(float position, T start, T end)
	{
		return static_cast<T>((end - start) * position * position * position + start);
	}

	template<class T>T ETween<T>::CubicOut(float position, T start, T end)
	{
		--position;
		return static_cast<T>((end - start) * (position * position * position + 1) + start);
	}
	
	template<class T>T ETween<T>::CubicInOut(float position, T start, T end)
	{
		position *= 2;
		if (position < 1)
			return static_cast<T>(((end - start) / 2) * position * position * position + start);

		position -= 2;
		return static_cast<T>(((end - start) / 2) * (position * position * position + 2) + start);
	}

	template<class T>T ETween<T>::QuintIn(float position, T start, T end)
	{
		return static_cast<T>((end - start) * position * position * position * position * position + start);
	}

	template<class T>T ETween<T>::QuintOut(float position, T start, T end)
	{
		--position;
		return static_cast<T>((end - start) * (position * position * position * position * position + 1) + start);
	}
	
	template<class T>T ETween<T>::QuintInOut(float position, T start, T end)
	{
		position *= 2;
		if(position < 1)
			return static_cast<T>(((end - start) / 2) * (position * position * position * position * position) + start);

		position -= 2;
		return static_cast<T>(
			((end - start) / 2) * (position * position * position * position * position + 2) + start
			);
	}

	template<class T>T ETween<T>::BackIn(float position, T start, T end)
	{
		float s = 1.70158f;
		float postFix = position;
		return static_cast<T>((end - start) * postFix * position * ((s + 1) * position - s) + start);
	}

	template<class T>T ETween<T>::BackOut(float position, T start, T end)
	{
		float s = 1.70158f;
		position--;
		return static_cast<T>((end - start) * (position * position * ((s + 1) * position + s) + 1) + start);
	}

	template<class T>T ETween<T>::BackInOut(float position, T start, T end)
	{
		float s = 1.70158f;
		float t = position;
		T b = start;
		T c = end - start;
		float d = 1;
		s *= 1.525f;
		if ((t /= d / 2) < 1)
			return static_cast<T>(c / 2 * (t * t * ((s + 1) * t - s)) + b);

		float postFix = t -= 2;
		return static_cast<T>(c / 2 * (postFix * t * ((s + 1) * t + s) + 2) + b);
	}
}