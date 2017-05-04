#pragma once

#include <functional>

namespace Elixir
{
	template <class T>
	struct tweenData
	{
	public:
		bool fromReady;
		bool byReference;
		T* initialValue;
		T initialCpy;
		T finalValue;
		float duration;
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

		std::vector<tweenData<T>> GetTweens();

		void Update(float dt);
	private:
		void AddTween(tweenData<T> tween);

	private:
		int m_lastTweenIndex;
		std::vector<tweenData<T>> m_tweenVec;
	};

	template<class T>ETween<T>::ETween()
		:m_lastTweenIndex(-1)
	{}

	template<class T>ETween<T>::~ETween()
	{}

	template<class T>ETween<T> ETween<T>::From(T* initVal)
	{
		tweenData<T> tween;

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

		for (auto &tween : m_tweenVec)
		{
			if (tween.fromReady)
			{
				auto value = tween.initialCpy + (tween.timeCounter / tween.duration) * (tween.finalValue - tween.initialCpy);
				*tween.initialValue = value;

				if (tween.timeCounter >= tween.duration)
				{
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
				}

				tween.timeCounter += dt;
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

	template<class T>std::vector<tweenData<T>> ETween<T>::GetTweens()
	{
		return m_tweenVec;
	}

	template<class T>void ETween<T>::AddTween(tweenData<T> tween)
	{
		m_tweenVec.push_back(tween);
	}
}