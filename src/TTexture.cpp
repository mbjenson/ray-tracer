#include "TTexture.h"

TTexture::TTexture(){
    width = -1;
    height = -1;
    maxColVal = -1;
}

TTexture::TTexture(std::string filename_){
    filename = filename_;
    width = -1;
    height = -1;
    maxColVal = -1;
}

int TTexture::load(){
    std::ifstream file(filename);
    if (!file.good()){
        std::cout << "invalid filename: " << filename << "\n";
        return 1;
    }
    if (file.is_open()){
        // Header
        std::string line;
        getline(file, line); // retreive header line
        const char* cLine = line.c_str();
        if (sscanf(cLine, "%*s %d %d %d", &width, &height, &maxColVal) != 3){ // parse the line
            std::cout << "invalid header in file: " << filename << "\n";
            return 1;
        }
        if (width < 1 || height < 1){
            std::cout << "invalid texture; must have dimensions greater than or equal to 1\n";
        }
        pixels.resize(width * height);
        // Pixels
        std::string word;
        int i = 0;
        while (i < (width * height)){
            TColor thisCol;
            for (int j = 0; j < 3; j++){
                if (file >> word){
                    int val = stoi(word);
                    if (j == 0){
                        thisCol.r = val;
                    }
                    else if (j == 1){
                        thisCol.g = val;
                    }
                    else if (j == 2){
                        thisCol.b = val;
                    }
                }
                else if (i < (width * height) - 1){ // if EOF reached but pixels are missing
                    std::cout << "Texture file [" << filename << "] has too few color values for it's resolution\n";
                    return 1;
                }
            }
            pixels[i] = thisCol;
            //pixels.push_back(thisCol); // push this color into the pixels vector
            i++;
        }
        file.close();
        loaded = true;
        return 0;
    }
    return 1;
}

TColor TTexture::getPix(int i, int j){
    if (loaded){
        if ((j * width + i) < (width * height)){
            return pixels[j * width + i];
        }
    }
    return TColor(); // if out of bounds, return black
}

/**
 * Calculates i and j (x and y) from u and v values then gets pixel color at i, j from pixels vector.
 * Values of u and v are clamped in range 0 and 1 to allow for texture wrapping.
*/
TColor TTexture::getPix(float u, float v){
    if (loaded){
        if (u >= 0 && v >= 0){
            // texture wrapping
            if (u > 1){
                u = u - static_cast<int>(u);
            }
            if (v > 1){
                v = v - static_cast<int>(v);
            }
            // calc pixel coords
            int i = round(u * (width - 1));
            int j = round(v * (height - 1));
            return getPix(i, j);
            std::cout << "pixel coordinates (i, j) out of bounds of texture\n";
        }
        std::cout << "invalid texture coordinates\n";
    }
    return TColor(); // if invalid values, return black
}

TColor TTexture::interp(float u, float v){
    if (loaded){
        float x = u * (width - 1);
        float y = v * (height - 1);
        int i = static_cast<int>(x);
        int j = static_cast<int>(y);
        float a = x - i;
        float b = y - j;
        TColor c1 = (1-a) * (1-b) * getPix(i, j);
        TColor c2 = a * (1-b) * getPix(i+1, j);
        TColor c3 = (1-a) * b * getPix(i, j+1);
        TColor c4 = a * b * getPix(i+1, j+1);   
        TColor f = c1 + c2 + c3 + c4;
        return f;
    }
    return TColor();
}