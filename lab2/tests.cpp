#include <gtest/gtest.h>
#include "Playlist.h"

TEST(PlaylistTests, Constructor_Expected_Exception_OnZeroCapacity) {
    EXPECT_THROW(Playlist p(0), PlaylistException);
}
TEST(PlaylistTests, Indexer_Expected_Exception_OnOutOfRange) {
    Playlist p(2);
    p.Add({"Song A", "Artist A", 200});
    EXPECT_THROW(p[5], PlaylistException);
}

TEST(PlaylistTests, Add_Expected_Exception_OnDuplicate) {
    Playlist p(2);
    p.Add({"Song A", "Artist A", 200});
    EXPECT_THROW(p.Add({"Song A", "Artist A", 250}), PlaylistException);
}

TEST(PlaylistTests, Add_Expected_Exception_OnOverflow) {
    Playlist p(1);
    p.Add({"Song A", "Artist A", 200});
    EXPECT_THROW(p.Add({"Song B", "Artist B", 180}), PlaylistException);
}
TEST(PlaylistTests, Equal_SamePlaylists_ReturnsTrue) {
    Playlist a(2);
    a.Add({"Song A", "Artist A", 200});
    Playlist b(2);
    b.Add({"Song A", "Artist A", 200});
    EXPECT_TRUE(a == b);
}
TEST(PlaylistTests, TotalDuration_ReturnsSumOfTracks) {
    Playlist p(2);
    p.Add({"Song A", "Artist A", 200});
    p.Add({"Song B", "Artist B", 100});
    EXPECT_EQ(p.TotalDuration(), 300);
}
// Тесты для Merge, RemoveTracksOf и FindByArtist — самостоятельно
TEST(PlaylistTests, Merge_AllUnique_AddsAll) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(3);
    other.Add({"miyagi_song", "miyagi", 200});
    other.Add({"radio_tapok", "tapok", 150});
    p.Merge(other);

    ASSERT_EQ(p.Count(), 3);
    EXPECT_EQ(p[1].title, "miyagi_song");
    EXPECT_EQ(p[2].title, "radio_tapok");
}
TEST(PlaylistTests, Merge_AllDuplicates_NothingChanges) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(3);
    other.Add({"morgenstern_song", "morgenstern", 100});
    p.Merge(other);

    EXPECT_EQ(p.Count(), 1);
}
TEST(PlaylistTests, Merge_MixedUniqueAndDuplicates_AddsOnlyUnique) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(3);
    other.Add({"morgenstern_song", "morgenstern", 100});
    other.Add({"miyagi_song", "miyagi", 200});
    p.Merge(other);

    ASSERT_EQ(p.Count(), 2);
    EXPECT_EQ(p[1].title, "miyagi_song");
}
TEST(PlaylistTests, Merge_NotEnoughCapacity_ThrowsAndStateUnchanged) {
    Playlist p(2);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(3);
    other.Add({"miyagi_song", "miyagi", 200});
    other.Add({"radio_tapok", "tapok", 150});

    EXPECT_THROW(p.Merge(other), PlaylistException);
    EXPECT_EQ(p.Count(), 1);
}
TEST(PlaylistTests, Merge_EmptyOther_NothingChanges) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(2);
    p.Merge(other);
    EXPECT_EQ(p.Count(), 1);
}
TEST(PlaylistTests, RemoveTracksOf_SomeMatch_RemovesOnlyThoseAndPreservesOrder) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    p.Add({"miyagi_song", "miyagi", 200});
    p.Add({"radio_tapok", "tapok", 150});
    Playlist other(2);
    other.Add({"miyagi_song", "miyagi", 200});
    p.RemoveTracksOf(other);

    ASSERT_EQ(p.Count(), 2);
    EXPECT_EQ(p[0].title, "morgenstern_song");
    EXPECT_EQ(p[1].title, "radio_tapok");
}
TEST(PlaylistTests, RemoveTracksOf_NoMatch_ThrowsAndStateUnchanged) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(2);
    other.Add({"unknown_song", "unknown_artist", 300});

    EXPECT_THROW(p.RemoveTracksOf(other), PlaylistException);
    EXPECT_EQ(p.Count(), 1);
}
TEST(PlaylistTests, RemoveTracksOf_AllMatch_EmptiesPlaylist) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    p.Add({"miyagi_song", "miyagi", 200});
    Playlist other(5);
    other.Add({"morgenstern_song", "morgenstern", 100});
    other.Add({"miyagi_song", "miyagi", 200});
    p.RemoveTracksOf(other);

    EXPECT_EQ(p.Count(), 0);
}
TEST(PlaylistTests, RemoveTracksOf_EmptyOther_Throws) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    Playlist other(2);

    EXPECT_THROW(p.RemoveTracksOf(other), PlaylistException);
}
TEST(PlaylistTests, FindByArtist_FoundAtFirstPosition) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    p.Add({"miyagi_song", "miyagi", 200});

    EXPECT_EQ(p.FindByArtist("morgenstern"), 0);
}
TEST(PlaylistTests, FindByArtist_FoundAtLastPosition) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});
    p.Add({"miyagi_song", "miyagi", 200});
    p.Add({"radio_tapok", "tapok", 150});

    EXPECT_EQ(p.FindByArtist("tapok"), 2);
}
TEST(PlaylistTests, FindByArtist_NotFound_Throws) {
    Playlist p(5);
    p.Add({"morgenstern_song", "morgenstern", 100});

    EXPECT_THROW(p.FindByArtist("unknown_artist"), PlaylistException);
}
TEST(PlaylistTests, FindByArtist_EmptyPlaylist_Throws) {
    Playlist p(5);

    EXPECT_THROW(p.FindByArtist("morgenstern"), PlaylistException);
}