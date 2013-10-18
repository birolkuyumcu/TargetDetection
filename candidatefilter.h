#ifndef CANDIDATEFILTER_H
#define CANDIDATEFILTER_H

#include <opencv2/opencv.hpp>
#include "exception.h"
#include <vector>
#include <list>

enum TargetState
{
    candidate,
    visible,
    invisible
};

class CandidateFilterSettings
{
public:
    int dummy;
};

class Target
{
public:
    cv::RotatedRect location;
    TargetState status;
    unsigned statusCounter;
    bool isMatched;

};

class MatchItem
{
public:
    int candidateIndex;
    int targetIndex;
    float distance;
    bool operator<(const MatchItem &other) const { return distance < other.distance; }
};

/*
 * Aday Listesini al
 ilk giriş ise hepsini  targetList ‘e  aday olarak ekle (status candidate)
 Nesne Listesi ile eşleştirme yap ( Eşletirme Kıstası Ne  ? )
 Eşleşen Nesnenin durumu aday ise belirginlik sayısını artır
 Belirginlik sayısı eşik değere ulaştıysa durumunu görülebilire çevir
 Eşleşenlerin konumlarını güncelle
 Eşleşme olmayan görülebilir nesnelerin görünmezlik sayısını artır
Görünmezlik sayısı eşik değeri aşmışsa nesneyi sil
 Eşleşmeyenleri  Nesne Listesine  aday olarak ekle

Eşleştirme ;
Her Aday için her nesneye olan uzaklığı hesapla
Her Adayın en yakın olduğu nesneye eğer eşik değerinden yakınsa ata
eşleştirilenleri listeden kaldır



 **/

class CandidateFilter
{
public:
    CandidateFilter();
    void set(CandidateFilterSettings& _settings);
    void getSettings(CandidateFilterSettings& _settings);
    void saveSettings();
    bool loadSettings();
    //void process(cv::Mat& inputImage);
    void process(std::vector<cv::RotatedRect> *iCandidateList);
    void processUnmatchedTargets();
    void processUnmatchedCandidates();
    void init();
    void match();
    void showTargets(cv::Mat& inputImage);
    //
    std::vector<Target> targetList;
    std::vector<cv::RotatedRect> *candidateList;
    std::vector<bool> isCandidateMatched;

private:
    CandidateFilterSettings settings;
    Exception               exc;
    float calculateDistance(cv::RotatedRect& r1,cv::RotatedRect& r2);
};

#endif // CANDIDATEFILTER_H
