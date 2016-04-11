#ifndef MESHLOADER_H
#define MESHLOADER_H
/**
 ** Leibniz Universität Hannover - Institute for Man-Machine-Communication
 ** Copyright (c) 2013-2016 Andreas Tarnowsky <tarnowsky@welfenlab.de>
 **
 ** You should have received a copy of the MIT License along with this program.
 **
 ** Changes:
 ** 2015/04 (r2) - More robust implementation, should be able to load all Blender exported PLY files now
 **
 **/


#include <QString>
#include <QVector>
#include "interfaces/Tuple3.h"


/**
 * @brief Die MeshLoader Klasse
 *
 * Ein (sehr einfacher) PLY-Datei Parser. Unterstüzt momentan NUR Dateien,
 * die mit Blender exportiert wurden und Vertex-Farben UND UV-Koordinaten
 * besitzen.
 *
 * Die Face / VertexInfo Structs werden der RendererBase-Instanz im meshChanged
 * Aufruf übergeben.
 *
 * Interne Klasse, bitte nicht direkt einbinden und/oder verändern!
 */
class MeshLoader
{
public:
    struct VertexInfo
    {
        float x, y, z;      // Position des Vertex
        float nx, ny, nz;   // Normale
        float u, v;         // UV-Coordinaten (Texturkoordinaten)
        float r, g, b;      // Farbe (Bereich: 0.0 ... 1.0)
    };

    typedef Tuple3<VertexInfo> Face;

    MeshLoader();
    MeshLoader(const QString& fileName);

    void parseFile();
    void parseVertexLine(const QStringList& line, MeshLoader::VertexInfo& v);
    bool isValid() const;

    const QVector<Face>& faces() const;

private:
    QVector<Face> _faces;
    bool _valid;
    QString fileName;
    QVector<int> _indexMap;
};

#endif // MESHLOADER_H
