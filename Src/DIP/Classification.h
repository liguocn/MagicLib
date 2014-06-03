#pragma once
#include <vector>

namespace MagicDIP
{
    class Classification
    {
    public:
        Classification();
        virtual ~Classification() = 0;
        
        virtual void LearnDictionary(void) = 0;
        virtual void LearnClassifier(void) = 0;
        virtual int Classify(const std::vector<double>& data) = 0;
    };
}
