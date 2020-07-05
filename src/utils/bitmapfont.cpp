// bitmap font class implementation

#include <fstream>
#include <nlohmann/json.hpp>
#include "utils/bitmapfont.hpp"

using json = nlohmann::json;

BitmapFont::BitmapFont(std::string configPath, SDL_Renderer * renderer) {
    buildFont(configPath, renderer);

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);
    screenWidth = dm.w;
    screenHeight = dm.h;
}

// build the font - load the texture + define the character clips
void BitmapFont::buildFont(std::string configPath, SDL_Renderer * renderer) {
    json configJSON;
    std::ifstream instream(configPath);
    instream >> configJSON;

    std::map<std::string, json> configMap = configJSON;

    // load the texture
    auto texturePaths = configMap[TEXTURE_LABEL].get<std::vector<json>>();

    // get path to config file directory
    std::size_t lastSepIdx = configPath.find_last_of(PATH_SEP);

    auto texturePath = configPath.substr(0, lastSepIdx + 1) +
        texturePaths.back().get<std::string>();
    bitmapTexture.loadBitmapTexture(texturePath, renderer);

    // retrieve common data
    auto commonData = configMap[COMMON_DATA_LABEL].get<std::map<std::string, json>>();
    
    newLineChar = commonData["lineHeight"].get<int>();
    
    // loop through each char in the bitmap, retrieve clips/spacings
    auto charMaps = configJSON[CHARMAP_LABEL].get<std::vector<std::map<std::string, int>>>();
    for(auto & charMap: charMaps) {
        charClips.emplace(charMap["id"], (struct SDL_Rect) {charMap["x"],
            charMap["y"], charMap["width"], charMap["height"]});

        charSpacings.emplace(charMap["id"], (struct CharSpacing)
            {charMap["xoffset"], charMap["yoffset"], charMap["xadvance"]});
    }
}

// call 1x to initialize variables for typed/flashing text rendering
void BitmapFont::initRenderDynamicText(int x, int y, const std::string & text, 
    bool typed, bool flashing) {

    renderingDynamic = true;
    renderX = x;
    renderY = y;
    currString = text;
    currChar = typed ? text.length() - 1 : 0;

    this->flashing = flashing;
}

// call this when rendering typed text
void BitmapFont::renderText(SDL_Renderer * renderer) const {
    // render each char in the string up to currChar
    renderText(renderer, currString, renderX, renderY, currChar);
}

// call this when rendering static text
void BitmapFont::renderText(SDL_Renderer * renderer, const std::string & text, 
    int x, int y) const {
    
    // render the entire string
    renderText(renderer, text, x, y, text.length() - 1);
}

// render text using the bitmap font
void BitmapFont::renderText(SDL_Renderer * renderer, const std::string & text,
    int x, int y, unsigned int lastCharIdx) const {

    int currX = x, currY = y;

    for(unsigned int i = 0; i <= lastCharIdx; i++) {
        // access correct clip using ascii val.
        int ascii = (unsigned char) text[i];

        // check if space char
        if(ascii == SPACE_CHAR) {
            currX += spaceChar;
        }

        // check if newline char or within pad distance of border
        if(ascii == NEWLINE_CHAR || currX > screenWidth - H_PAD) {
            // move down and back
            currY += newLineChar;
            currX = x;
            continue;
        }

        currX += charSpacings.at(ascii).charXOffset;
        currY += charSpacings.at(ascii).charYOffset;

        bitmapTexture.render(currX, currY + charSpacings.at(ascii).charYOffset,
            renderer, &charClips.at(ascii));

        // add padding between chars
        currX += charSpacings.at(ascii).charXAdvance;
    }
}


// if already rendering typed text call these functions
void BitmapFont::updateText(float delta) {
    // check if already reached end of string - update alpha
    if(currChar == currString.length() - 1) {
        if(flashing) updateAlpha();
        
        return;
    }

    // update string progress/current character count based on time
    stringProgress += textSpeed * (delta / 1000);

    currChar = (int) stringProgress;
    if(currChar > currString.length() - 1) {
        currChar = currString.length() - 1;
    }
}

void BitmapFont::setRenderingDynamic(bool renderingDynamic) {
    this->renderingDynamic = renderingDynamic;
}

void BitmapFont::updateAlpha() {
    bitmapTexture.updateAlpha();
}

void BitmapFont::setFontColor(const SDL_Color & fontColor) {
    bitmapTexture.setColor(fontColor.r, fontColor.g, fontColor.b);
}

void BitmapFont::setAlpha(Uint8 a) {
    bitmapTexture.setAlpha(a);
}

bool BitmapFont::isRenderingDynamic() const {
    return renderingDynamic;
}

int BitmapFont::getLineHeight() const {
    return newLineChar;
}

// get height of text in pixels
int BitmapFont::getTextHeight(const std::string & text) const {
    int height = getLineHeight();

    auto find = text.find(NEWLINE_CHAR);
    while(find != std::string::npos) {
        height += getLineHeight();
        find = text.find(NEWLINE_CHAR, find + 1);
    }

    return height;
}

// get width of text in pixels (if multiple lines, returns the largest)
int BitmapFont::getTextWidth(const std::string & text) const {
    int maxWidth = 0;
    int width = 0;

    for(unsigned int i = 0; i < text.length(); i++) {
        int ascii = (unsigned char) text.at(i);

        if(ascii == NEWLINE_CHAR) {
            if(width > maxWidth) {
                maxWidth = width;
            }
            width = 0;
            continue;
        }

        if(ascii == SPACE_CHAR) {
            width += spaceChar;
        }

        width += charSpacings.at(ascii).charXOffset + 
            charSpacings.at(ascii).charXAdvance;
    }

    // check final line
    maxWidth = width > maxWidth ? width : maxWidth;
    return maxWidth;
}