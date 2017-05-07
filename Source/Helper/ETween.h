#pragma once

#include <functional>
#include <algorithm>

namespace Elixir
{
	template <class T>
	struct tweenData
	{
	public:
		tweenData(int tID)
			:tweenID(tID)
		{}

		inline bool operator==(const tweenData& td2)
		{
			return this->tweenID == td2.tweenID;
		}

		int tweenID;
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

		void AddTweens(std::vector<tweenData<T>> tweens);

		void Update(float dt);
		void AddTween(tweenData<T> tween);
	private:
		

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
}