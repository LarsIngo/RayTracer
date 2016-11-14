#pragma once

// Entity entry.
struct EntityEntry
{
    // Index to first vertex in vertex buffer.
    int offset;
    // Number of vertices in entity.
    int numOfVertices;
    float pad[2];

    // Constructor.
    EntityEntry();

    // Constructor.
    // pos Position.
    // col Color.
    EntityEntry(int offset, int numOfVertices);
};

inline EntityEntry::EntityEntry()
{
    offset = -1;
    numOfVertices = -1;
}

inline EntityEntry::EntityEntry(int offset, int numOfVertices)
{
    this->offset = offset;
    this->numOfVertices = numOfVertices;
}
