#include "Playlist.h"
#include <sstream>
#include <iomanip>

Playlist::Playlist(int capacity) {
    if (capacity <= 0)
        throw PlaylistException("недопустимое значение вместимости = " + std::to_string(capacity));
        this->capacity = capacity;
        this->count = 0;
        this->tracks = new Track[capacity];
}

Playlist::Playlist(const Playlist& other) {
    capacity = other.capacity;
    count = other.count;
    tracks = new Track[capacity];
    for (int i = 0; i < count; i++)
        tracks[i] = other.tracks[i];
}

Playlist& Playlist::operator=(const Playlist& other) {
    if (this == &other)
        return *this;
    delete[] tracks;
    capacity = other.capacity;
    count = other.count;
    tracks = new Track[capacity];
    for (int i = 0; i < count; i++)
        tracks[i] = other.tracks[i];
        return *this;
}

Playlist::~Playlist() {
    delete[] tracks;
}
const Track& Playlist::operator[](int i) const {
   if (i < 0 || i > count - 1)
        throw PlaylistException("неверное значение индекса i = " + std::to_string(i));
        return tracks[i];
}

Track& Playlist::operator[](int i) {
   if (i < 0 || i > count - 1)
       throw PlaylistException("неверное значение индекса i = " + std::to_string(i));
       return tracks[i];
}

bool Playlist::operator==(const Playlist& other) const {
    if (count != other.count)
        return false;
    for (int i = 0; i < count; i++) {
        if (tracks[i] != other.tracks[i])
            return false;
    }
    return true;
}

bool Playlist::Contains(const Track& track) const {
    for (int i = 0; i < count; i++) {
        if (tracks[i] == track)
            return true;
        }
    return false;
}

void Playlist::Add(const Track& track) {
    if (count >= capacity)
        throw PlaylistException("плейлист заполнен, вместимость = " + std::to_string(capacity));
    if (Contains(track))
        throw PlaylistException("трек уже есть в плейлисте: " + track.artist + " - " + track.title);
        tracks[count] = track;
        count++;
}

int Playlist::TotalDuration() const {
    int total = 0;
    for (int i = 0; i < count; i++)
        total += tracks[i].durationSec;
    return total;
}

std::string Playlist::ToString() const {
    std::ostringstream out;
    for (int i = 0; i < count; i++) {
        int m = tracks[i].durationSec / 60;
        int s = tracks[i].durationSec % 60;
        out << tracks[i].artist << " - " << tracks[i].title << " (" << m << ":" << setw(2) << setfill('0') << s << ")\n";
    }
    return out.str();
}

// Merge, RemoveTracksOf и FindByArtist оставлены как самостоятельные
void Playlist::Merge(const Playlist &other){
    int toAdd = 0;
    for (int i = 0; i < other.count; i++) {
        if (!Contains(other.tracks[i])) {toAdd++;}
    }

    if (count + toAdd > capacity){
        throw PlaylistException("недостаточно места, требуется = " + std::to_string(count + toAdd));
    }
    for (int i = 0; i < other.count; i++) {
        if (!Contains(other.tracks[i])) {Add(other.tracks[i]);}
    }
}  
void Playlist::RemoveTracksOf(const Playlist &other){
    bool found = false;
    Track* temp = new Track[capacity];
    int newCount = 0;

    for (int i = 0; i < count; i++) {
        bool shouldRemove = false;
        for (int j = 0; j < other.count; j++) {
            if (tracks[i] == other.tracks[j]) {
                shouldRemove = true;
                found = true;
                break;
            }
        }
        if (!shouldRemove) {
            temp[newCount] = tracks[i];
            newCount++;
        }
    }
    if (!found) {
        delete[] temp;
        throw PlaylistException("ни один трек из other не найден в текущем плейлисте");
    }
    delete[] tracks;
    tracks = temp;
    count = newCount;
}
int Playlist::FindByArtist(const string &artist) const{
    for (int i = 0; i < count; i++) {
        if (tracks[i].artist == artist){return i;}
    }
    throw PlaylistException("трек исполнителя не найден: " + artist);
}