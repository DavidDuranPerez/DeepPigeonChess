#include <gtest/gtest.h>
#include "../src/board.h"

namespace {
    // The fixture for testing class Foo.
    class BoardTest : public ::testing::Test {
        protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        BoardTest() {
            // You can do set-up work for each test here.
        }

        ~BoardTest() override {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        void TearDown() override {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test suite for Foo.
    };


    // Test to set a square
    TEST(BoardTest, SetSquare) {
        // Create the square
        Square sq = Square(2, 'a', 'R', true, false, false);

        // Get the piece
        EXPECT_EQ(sq.get_piece(), 'R') << "Erroneous piece set";
        // Set another piece
        sq.set_piece('B');
        EXPECT_EQ(sq.get_piece(), 'B') << "Erroneous piece set";
        // Get the column and row
        EXPECT_EQ(sq.get_col(), 'a') << "Erroneous column";
        EXPECT_EQ(sq.get_row(), 2) << "Erroneous row";
    }

    // Test to set a board
    TEST(BoardTest, SetBoard){
        // Create the board with the starting position
        Board bd = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

        // Get the pieces of some squares
        EXPECT_EQ(bd.get_piece(2, 3), 'N') << "Erroneous white piece";
        EXPECT_EQ(bd.get_piece(9, 2), 'r') << "Erroneous black piece";
    }
}  // namespace


// Run the tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}