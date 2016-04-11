/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 **/


#include "meshloader.h"
#include <QDebug>

#include <QFile>
#include <QStringList>
#include <QHash>

struct FaceOrder
{
    int a, b, c;
};

inline static QString prep(const QString& str)
{
    return str.trimmed().toLower();
}

inline static QString nextLine(QFile& file)
{
    QString line = prep(QString(file.readLine()));
    while(line.contains("comment")) // Skip comments and unsupported lines
    {
        line = prep(QString(file.readLine()));
    }

    return line;
}

inline static QStringList nextTuple(QFile& file)
{
    QString line = nextLine(file);
    return line.split(' ', QString::SkipEmptyParts);
}

void MeshLoader::parseVertexLine(const QStringList& line, MeshLoader::VertexInfo& v)
{
    const float invColor = 1.0f/255.0f;
    float data[11] = {0.5f};
    int maxIndex = qMin(_indexMap.count(), line.count());

    for(int i = 0; i < maxIndex; i++)
    {
        data[_indexMap[i]] = line[i].toFloat();
    }

    v.x = data[0]; v.y = data[1]; v.z = data[2];
    v.nx = data[3]; v.ny = data[4]; v.nz = data[5];
    v.u = data[6]; v.v = data[7];
    v.r = data[8] * invColor; v.g = data[9] * invColor; v.b = data[10] * invColor;
}



MeshLoader::MeshLoader(const QString& fileName) : fileName(fileName)
{
    _valid = false;
}

MeshLoader::MeshLoader()
{
    _valid = false;
}

void MeshLoader::parseFile()
{
    _valid = false;
    _faces.clear();

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << "File " << fileName << " does not exist or is not readable.";
        return;
    }


    QString firstLine = nextLine(file);
    if(firstLine != "ply")
    {
        qCritical() << "File " << fileName << " is not a PLY file.";
        return;
    }

    QStringList format = nextTuple(file);
    if(format.size() != 3 &&
       format.at(0).startsWith("format") &&
       format.at(1).startsWith("ascii"))
    {
        qCritical() << "Only ascii formatted PLY files are supported right now.";
        return;
    }

    // Setup index-map
    _indexMap.resize(11);
    for(int n = 0; n < 11; n++)
        _indexMap[n] = n;

    //_indexMap.clear();

    int numberOfVertices = 0;
    int numberOfFaces = 0;
    _indexMap.clear();

    QStringList nextLine;
    nextLine << "Init";

    QHash<QString, int> indexIdentifiers;
    indexIdentifiers["x"] = 0;
    indexIdentifiers["y"] = 1;
    indexIdentifiers["z"] = 2;
    indexIdentifiers["nx"] = 3;
    indexIdentifiers["ny"] = 4;
    indexIdentifiers["nz"] = 5;
    indexIdentifiers["s"] = 6;
    indexIdentifiers["t"] = 7;
    indexIdentifiers["red"] = 8;
    indexIdentifiers["green"] = 9;
    indexIdentifiers["blue"] = 10;

    while(nextLine.size() > 0 && !nextLine[0].startsWith("end_header"))
    {
        nextLine = nextTuple(file);

        if(nextLine.size() >= 3 &&
           nextLine.at(0).startsWith("element") &&
           nextLine.at(1).startsWith("vertex"))
        {
            numberOfVertices = nextLine.at(2).toInt();
        }

        if(nextLine.size() >= 3 &&
           nextLine.at(0).startsWith("element") &&
           nextLine.at(1).startsWith("face"))
        {
            numberOfFaces = nextLine.at(2).toInt();
        }

        if(nextLine.size() == 3 &&
           nextLine.at(0).startsWith("property"))
        {
            _indexMap << indexIdentifiers.value(nextLine.at(2), -1);
        }
    }

    QVector<VertexInfo> allVertices;
    for(int n = 0; n < numberOfVertices; n++)
    {
        VertexInfo nextVertex;
        parseVertexLine(nextTuple(file), nextVertex);
        allVertices.append(nextVertex);
    }

    if(allVertices.size() != numberOfVertices)
    {
        qWarning() << "Looks like something went wrong... A total of" << allVertices.size() << "vertices were found, but there should be" << numberOfVertices << ". I'll try my best, but you should check the file format!";
    }


    // Next: Faces
    QVector<FaceOrder> faceReferences;
    for(int n = 0; n < numberOfFaces; n++)
    {
        FaceOrder f;
        QStringList line = nextTuple(file);

        if(line.size() == 4 && line.at(0).toInt() == 3)
        {
            f.a = line.at(1).toInt();
            f.b = line.at(2).toInt();
            f.c = line.at(3).toInt();

            if(f.a < numberOfVertices && f.b < numberOfVertices && f.c < numberOfVertices)
            {
                faceReferences.append(f);
            }
            else
            {
                qWarning() << "Malformed PLY File. Vertex index out of bounds. Skipping face.";
            }
        }
        else
        {
            qWarning() << "Malformed PLY File. Expecting a tuple of 4 elements containing vertex references. Got a" << line.size() << "tuple. Skipping entry.";
        }
    }

    if(faceReferences.size() != numberOfFaces)
    {
        qWarning() << "Looks like something went wrong... A total of " << allVertices.size() << " vertices were found, but there should be " << numberOfVertices << ". I'll try my best, but you should check the file format!";
    }


    // Convert to internal format

    foreach (FaceOrder f, faceReferences)
    {
        Face face;

        face[0] = allVertices.at(f.a);
        face[1] = allVertices.at(f.b);
        face[2] = allVertices.at(f.c);

        _faces.append(face);
    }

    _valid = true;
}

bool MeshLoader::isValid() const
{
    return _valid;
}

const QVector<MeshLoader::Face>& MeshLoader::faces() const
{
    return _faces;
}
