#ifndef PREPROCESS_H
#define PREPROCESS_H

enum PreprocessMethods
{
    HistEq,
    GoF
};

class PreprocessSettings
{
public:
    PreprocessMethods method;
};

class Preprocess
{
public:
    Preprocess();
    void set(PreprocessSettings& _settings);

private:
    PreprocessSettings settings;
};


#endif // PREPROCESS_H
