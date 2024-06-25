#include <iostream>
#include <string>
#include "highSpeedTrain.h"


HighSpeedTrain::HighSpeedTrain(){
    from= "Florence";
    to="Rome";
    eta = 90;
}

HighSpeedTrain::HighSpeedTrain(std::string from, std::string to, int eta){
    this->from=from;
    this->to=to;
    this->eta=eta;
}

HighSpeedTrain::HighSpeedTrain(const HighSpeedTrain &h){
    this->from=h.from;
    this->to=h.to;
    this->eta=h.eta+60;
}

std::ostream &operator<<(std::ostream &output, const HighSpeedTrain &h){
    output<< h.from <<" -> "<<h.to<<" in "<< h.eta <<" mins.";
    return output;
}


std::string operator+(const HighSpeedTrain &h1, const HighSpeedTrain &h2){
    if(h1.to==h2.from)
         return (h1.from +" -> "+h1.to+" -> "+ h2.to +" in "+ std::to_string(h2.eta+h1.eta) +" mins.");
    else
        return "Transfer is not possible!";
    
}

/*
 * Comparison Overloads
 * This operator will be used to order the summaries of train.
 * The priority for comparison is here:
 * 1. Compare the source of the trains alphabetically (ex. Florence < Milano)
 * 2. Compare the destination of the trains alphabetically (the same example above)
 * 3. Compare the eta's of the trains, the train with less eta will be smaller.
 *
 * @param h1, h2 HighSpeedTrain the trains to be compared.
 */
bool operator<(const HighSpeedTrain &h1, const HighSpeedTrain &h2){
    if(h1.from != h2.from)
        return (h1.from<h2.from);
    else if (h1.to!=h2.to)
        return (h1.to<h2.to);
    else
        return (h1.eta<h2.eta);
}

bool operator>(const HighSpeedTrain &h1, const HighSpeedTrain &h2){
    if(h1.from != h2.from)
        return (h1.from>h2.from);
    else if (h1.to!=h2.to)
        return (h1.to>h2.to);
    else
        return (h1.eta>h2.eta);
}









