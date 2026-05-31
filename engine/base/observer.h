#pragma once
#include <vector>
#include <algorithm>

class Observer
{
public:
    virtual ~Observer() = default;
};

template<typename ObserverType>
class Subject
{
public:
    virtual ~Subject() = default;

    void attach(ObserverType* observer)
    {
        if (!observer)
            return;

        auto iter = std::find(_observers.begin(), _observers.end(), observer);
        if (iter != _observers.end())
            return;

        _observers.push_back(observer);
    }

    void detach(ObserverType* observer)
    {
        _observers.erase(
            std::remove(_observers.begin(), _observers.end(), observer),
            _observers.end()
        );
    }

protected:
    const std::vector<ObserverType*>& observers() const
    {
        return _observers;
    }

private:
    std::vector<ObserverType*> _observers;
};