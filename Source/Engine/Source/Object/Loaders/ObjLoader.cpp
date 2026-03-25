// ◦ Xyz ◦

#include "ObjLoader.h"
#include <sstream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <Logs.h>

namespace {
    static size_t GetCount(std::string_view word, size_t& offsetPos, const std::string& data) {
        if (data.empty()) {
            return 0;
        }

        size_t posEnd = data.rfind(word, offsetPos);
        if (posEnd == data.npos) {
            return 0;
        }

        offsetPos = data.rfind('#', posEnd);
        if (offsetPos == data.npos) {
            return 0;
        }

        size_t posBegin = data.find_first_not_of(' ', offsetPos + 1);
        if (posBegin == data.npos) {
            return 0;
        }

        posEnd = data.find_first_of(' ', posBegin + 1);
        if (posEnd == data.npos) {
            return 0;
        }

        size_t size = posEnd - posBegin;
        std::size_t count;
        const char* chBegin = data.data() + posBegin;
        const char* chEnd = chBegin + size;
        auto [ptr, ec] = std::from_chars(chBegin, chEnd, count);

        // TODO:
        if (ec != std::errc()) {
            return 0;
        }

        return count;
    }
}

Engine::Mesh Engine::Loader::ObjLoad(const std::string& data)
{
    size_t pos = data.size();

    size_t countFace = GetCount("faces", pos, data);
    if (countFace == 0) {
        LOG("[Loader::ObjLoad] faces count is 0.");
        return {};
    }

    size_t countTexCoord = GetCount("texture coords", pos, data);
    if (countTexCoord == 0) {
        LOG("[Loader::ObjLoad] texture coords count is 0.");
    }

    size_t countNormal = GetCount("vertex normals", pos, data);
    if (countNormal == 0) {
        LOG("[Loader::ObjLoad] vertex normals count is 0.");
    }

    size_t countVertex = GetCount("vertices", pos, data);
    if (countVertex == 0) {
        LOG("[Loader::ObjLoad] vertices count is 0.");
        return {};
    }

    std::vector<float> readVertices;
    readVertices.reserve(countVertex);
    std::vector<float> readTexCoords;
    readTexCoords.reserve(countTexCoord);
    std::vector<float> readNormals;
    readNormals.reserve(countNormal);

    countFace *= 3;
    std::unique_ptr<float[]> vertexes = std::make_unique<float[]>(countFace * 3);
    std::unique_ptr<float[]> texCoords = countTexCoord > 0 ? std::make_unique<float[]>(countFace * 2) : nullptr;
    std::unique_ptr<float[]> normals = countNormal > 0 ? std::make_unique<float[]>(countFace * 3) : nullptr;

    size_t indexVertex = 0;
    size_t indexTexCoord = 0;
    size_t indexNormal = 0;

    std::istringstream stream(data);
    std::string line;

    while (std::getline(stream, line)) {
        std::istringstream ls(line);
        std::string prefix;
        ls >> prefix;

        if (prefix == "v") {
            float x, y, z;
            ls >> x >> y >> z;
            readVertices.emplace_back(x);
            readVertices.emplace_back(y);
            readVertices.emplace_back(z);
        }
        else if (prefix == "vt") {
            float u, t;
            ls >> u >> t;
            readTexCoords.emplace_back(u);
            readTexCoords.emplace_back(t);
        }
        else if (prefix == "vn") {
            float x, y, z;
            ls >> x >> y >> z;
            readNormals.emplace_back(x);
            readNormals.emplace_back(y);
            readNormals.emplace_back(z);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; i++) {
                std::string vert;
                ls >> vert;

                int vi = 0, ti = 0, ni = 0;
                if (sscanf(vert.c_str(), "%d/%d/%d", &vi, &ti, &ni) != 3) {
                    LOG("[Loader::ObjLoad] fail parse.");
                    return {};
                }

                --vi; --ti; --ni;

                // TODO: Проверка на выход за пределы массива
                vertexes[indexVertex++] = readVertices[vi * 3 + 0];
                vertexes[indexVertex++] = readVertices[vi * 3 + 1];
                vertexes[indexVertex++] = readVertices[vi * 3 + 2];

                if (!readTexCoords.empty() && texCoords) {
                    texCoords[indexTexCoord++] = readTexCoords[ti * 2 + 0];
                    texCoords[indexTexCoord++] = readTexCoords[ti * 2 + 1];
                }

                if (!readNormals.empty() && normals) {
                    normals[indexNormal++] = readNormals[ni * 3 + 0];
                    normals[indexNormal++] = readNormals[ni * 3 + 1];
                    normals[indexNormal++] = readNormals[ni * 3 + 2];
                }
            }
        }
    }

    return Engine::Mesh(countFace, vertexes.release(), normals.release(), texCoords.release());
}
