#pragma once

// Entity entry.
struct EntityEntry
{
    // Index to first vertex in vertex buffer.
    int offset;
    // Number of vertices in entity.
    int numVertices;
    float pad[2];

    // Constructor.
    EntityEntry();

    // Constructor.
    // pos Position.
    // col Color.
    EntityEntry(int offset, int numVertices);
};

inline EntityEntry::EntityEntry()
{
    offset = -1;
    numVertices = -1;
}

inline EntityEntry::EntityEntry(int offset, int numVertices)
{
    this->offset = offset;
    this->numVertices = numVertices;
}
