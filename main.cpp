#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstdlib>

using namespace std;

// UBX data types
#define u1 8    //bits
#define u2 16   //bits
#define u4 32   //bits

#define U1 uint8_t
#define U2 uint16_t
#define U4 uint32_t


void binaryRead(ifstream &binaryFile);

ifstream fileOpen(string filePath);

vector<bool> charArray2Bitset(const char *arr, int arraySize);

wstring readUBloxFile(const string &filePath);

void setUbxHeader(const wstring &fileContent, int &index, U1 &preamble1, U1 &preamble2, U1 &messageClass,
                  U1 &messageId, U2 &length);

string concatBytes(const wstring &fileContent, int begin, int finish);

void setPayload(const wstring &ubxData, int &index, U2 length);

void showResults(bool show);

int main() {


    showResults(true);

    const string filePath = R"(C:\Users\PC_901_ADMIN\Desktop\Projects\GPSL1Band\COM6_210713_094724.ubx)";
    wstring ubxData = readUBloxFile(filePath);

    int index = 0;
    U1 preamble1, preamble2, messageClass, messageId;
    U2 length;
    setUbxHeader(ubxData, index, preamble1, preamble2, messageClass, messageId, length);

    // set payload header
    setPayload(ubxData, index, length);

    return 0;
}


void showResults(bool show) { (show != true) ? cout.setstate(ios_base::failbit) : cout.clear(); }

void setPayload(const wstring &ubxData, int &index, U2 length) {
    int payloadHeaderLength = 8;
    bitset<u1> gnssIdBits(ubxData[++index]);
    bitset<u1> svIdBits(ubxData[++index]);
    bitset<u1> reserved0Bits(ubxData[++index]);
    bitset<u1> freqIdBits(ubxData[++index]);
    bitset<u1> numWordsBits(ubxData[++index]);
    bitset<u1> chnBits(ubxData[++index]);
    bitset<u1> versionBits(ubxData[++index]);
    bitset<u1> reserved1Bits(ubxData[++index]);

    U1 gnssId = gnssIdBits.to_ulong();
    U1 svId = svIdBits.to_ulong();
    U1 reserved0 = reserved0Bits.to_ulong();
    U1 freqId = freqIdBits.to_ulong();
    U1 numWords = numWordsBits.to_ulong();
    U1 chn = chnBits.to_ulong();
    U1 version = versionBits.to_ulong();
    U1 reserved1 = reserved1Bits.to_ulong();

    cout << "****************************************************" << endl;
    cout << "************** |----- PAYLOAD -----| ***************" << endl;
    cout << "****************************************************" << endl;
    cout << "gnssId : " << gnssIdBits.to_string() << " - " << (int) gnssId << endl;
    cout << "svId : " << svIdBits.to_string() << " - " << (int) svId << endl;
    cout << "reserved0 : " << reserved0Bits.to_string() << " - " << (int) reserved0 << endl;
    cout << "freqId : " << freqIdBits.to_string() << " - " << (int) freqId << endl;
    cout << "numWords : " << numWordsBits.to_string() << " - " << (int) numWords << endl;
    cout << "chn : " << chnBits.to_string() << " - " << (int) chn << endl;
    cout << "version : " << versionBits.to_string() << " - " << (int) version << endl;
    cout << "reserved1 : " << reserved1Bits.to_string() << " - " << (int) reserved1 << endl;
    cout << "*****************************************************" << endl << endl;


    cout << "****************************************************" << endl;
    cout << "************ |----- DATA WORDS -----| **************" << endl;
    cout << "****************************************************" << endl;


    U4 *dwrd = new U4[numWords];

    for (int i = 0; i < numWords; ++i) {

        int begin = ++index;
        int finish = index += (length - payloadHeaderLength);

        string temp = concatBytes(ubxData, begin, finish);
        bitset<u4> temp2(temp);
        dwrd[i] = temp2.to_ulong();
        cout << "dwrd[" << i << "] : " << temp2.to_string() << " - " << (int) dwrd[i] << endl;
    }

    cout << "*****************************************************" << endl << endl;

}


void setUbxHeader(const wstring &fileContent, int &index, U1 &preamble1, U1 &preamble2, U1 &messageClass,
                  U1 &messageId, U2 &length) {

    bitset<u1> preamble1Bits(fileContent[index]);
    bitset<u1> preamble2Bits(fileContent[++index]);
    bitset<u1> messageClassBits(fileContent[++index]);
    bitset<u1> messageIdBits(fileContent[++index]);
    int len0 = ++index;
    int len1 = ++index;
    bitset<u2> lengthBits(concatBytes(fileContent, len0, len1));

    preamble1 = (int) preamble1Bits.to_ulong();
    preamble2 = (int) preamble2Bits.to_ulong();
    messageClass = (int) messageClassBits.to_ulong();
    messageId = (int) messageIdBits.to_ulong();
    length = (int) lengthBits.to_ulong();

    cout << "****************************************************" << endl;
    cout << "************** |----- HEADER -----| ****************" << endl;
    cout << "****************************************************" << endl;
    cout << "premb1 : " << preamble1Bits.to_string() << " - " << (int) preamble1 << endl;
    cout << "premb2 : " << preamble2Bits.to_string() << " - " << (int) preamble2 << endl;
    cout << "mclass : " << messageClassBits.to_string() << " - " << (int) messageClass << endl;
    cout << "mid    : " << messageIdBits.to_string() << " - " << (int) messageId << endl;
    cout << "length : " << lengthBits.to_string() << " - " << (int) length << endl;
    cout << "*****************************************************" << endl << endl;
}

string concatBytes(const wstring &fileContent, const int begin, const int finish) {
    string concatedBits;
    for (int i = begin; i <= finish; ++i)
        concatedBits += bitset<u1>(fileContent[i]).to_string();

    return concatedBits;
}

wstring readUBloxFile(const string &filePath) {
    wifstream stream(filePath, ios::binary);
    if (!stream.is_open())
        exit(1);

    wstring line;
    wstring fileContent;

    while (getline(stream, line))
        fileContent += line;

    stream.close();
    return fileContent;
}


void binaryRead(ifstream &binaryFile) {
    int wordBytes = 4;
    int wordNumber = 10;
    int sizeOfSubframe = wordNumber * wordBytes * sizeof(char);

    char *word = new char[wordBytes * wordNumber];

    binaryFile.read(word, sizeOfSubframe);
    binaryFile.close();

    vector<bool> bits = charArray2Bitset(word, 4);

    int subframeWords[] = {8, 6, 6, 12, 5, 1, 26, 32, 20, 12, 12, 20};
    int wordsCounter = 0;
    for (int i = 0; i < wordBytes * wordNumber * 8; ++i) {
        if (!(i % 32)) cout << endl;
        cout << bits[i];
    }
}

ifstream fileOpen(const string filePath) {

    ifstream binaryFile(filePath, ios::binary);
    if (!binaryFile) {
        cerr << "File can not opened." << endl;
        exit(EXIT_FAILURE);
    } else cout << "File was opened." << endl;

    return binaryFile;
}

vector<bool> charArray2Bitset(const char *arr, const int arraySize) {

    vector<bool> bits;
    const int bitsSizeOfByte = 8;
    for (int i = 0; i < arraySize; i++)
        for (int j = 0; j < bitsSizeOfByte; ++j) {
            bits.push_back(((arr[i] >> j) & 1) != 0);
        }

    return bits;
}
