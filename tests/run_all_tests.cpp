#include <gtest/gtest.h>

#include "../inc/tmplInc/Hanoi.hpp"
#include "../inc/tmplInc/Stack.hpp"
#include "../inc/tmplInc/Flist.hpp"
#include "../inc/tmplInc/StackSequence.hpp"
#include "../inc/tmplInc/FlistSequence.hpp"

class HanoiTest : public ::testing::Test {
protected:
    Hanoi* hanoi;
    
    void SetUp() override {
        hanoi = new Hanoi(3);
    }
    
    void TearDown() override {
        delete hanoi;
    }
};

TEST_F(HanoiTest, InitializationTest) {
    EXPECT_EQ(hanoi->getPole(1)->getSize(), 3);
    EXPECT_EQ(hanoi->getPole(2)->getSize(), 0);
    EXPECT_EQ(hanoi->getPole(3)->getSize(), 0);
}

TEST_F(HanoiTest, CustomDiskCountTest) {
    Hanoi customHanoi(5);
    EXPECT_EQ(customHanoi.getPole(1)->getSize(), 5);
    EXPECT_EQ(customHanoi.getPole(2)->getSize(), 0);
    EXPECT_EQ(customHanoi.getPole(3)->getSize(), 0);
}

TEST_F(HanoiTest, SolveTest) {
    hanoi->solve(3, 1, 3);
    EXPECT_EQ(hanoi->getPole(1)->getSize(), 0);
    EXPECT_EQ(hanoi->getPole(2)->getSize(), 0);
    EXPECT_EQ(hanoi->getPole(3)->getSize(), 3);
}

TEST_F(HanoiTest, SingleDiskSolveTest) {
    Hanoi singleDiskHanoi(1);
    singleDiskHanoi.solve(1, 1, 3);
    EXPECT_EQ(singleDiskHanoi.getPole(1)->getSize(), 0);
    EXPECT_EQ(singleDiskHanoi.getPole(2)->getSize(), 0);
    EXPECT_EQ(singleDiskHanoi.getPole(3)->getSize(), 1);
}

TEST_F(HanoiTest, TwoDiskSolveTest) {
    Hanoi twoDiskHanoi(2);
    twoDiskHanoi.solve(2, 1, 3);
    EXPECT_EQ(twoDiskHanoi.getPole(1)->getSize(), 0);
    EXPECT_EQ(twoDiskHanoi.getPole(2)->getSize(), 0);
    EXPECT_EQ(twoDiskHanoi.getPole(3)->getSize(), 2);
}

TEST_F(HanoiTest, GetPoleTest) {
    EXPECT_NE(hanoi->getPole(1), nullptr);
    EXPECT_NE(hanoi->getPole(2), nullptr);
    EXPECT_NE(hanoi->getPole(3), nullptr);
    EXPECT_EQ(hanoi->getPole(4), hanoi->getPole(1));
    EXPECT_EQ(hanoi->getPole(0), hanoi->getPole(1));
}

class StackTest : public ::testing::Test {
protected:
    Stack<int>* stack;
    
    void SetUp() override {
        stack = new Stack<int>();
    }
    
    void TearDown() override {
        delete stack;
    }
};

TEST_F(StackTest, InitiallyEmpty) {
    EXPECT_TRUE(stack->isEmpty());
    EXPECT_EQ(stack->getSize(), 0);
}

TEST_F(StackTest, PushElements) {
    stack->push(1);
    stack->push(2);
    stack->push(3);
    EXPECT_EQ(stack->getSize(), 3);
}

TEST_F(StackTest, PopElements) {
    stack->push(1);
    stack->push(2);
    EXPECT_EQ(stack->getTop(), 2);
    stack->pop();
    EXPECT_EQ(stack->getSize(), 1);
    EXPECT_EQ(stack->getTop(), 1);
    stack->pop();
    EXPECT_EQ(stack->getSize(), 0);
}

TEST_F(StackTest, CopyConstructor) {
    stack->push(1);
    stack->push(2);
    Stack<int> copy(*stack);
    EXPECT_EQ(copy.getSize(), 2);
    EXPECT_EQ(copy.getTop(), 2);
    copy.pop();
    EXPECT_EQ(copy.getTop(), 1);
    copy.pop();
    EXPECT_EQ(stack->getSize(), 2);
}

TEST_F(StackTest, AssignmentOperator) {
    stack->push(1);
    stack->push(2);
    Stack<int> other;
    other = *stack;
    EXPECT_EQ(other.getSize(), 2);
    EXPECT_EQ(other.getTop(), 2);
    other.pop();
    EXPECT_EQ(other.getTop(), 1);
    other.pop();
    EXPECT_EQ(stack->getSize(), 2);
}

TEST_F(StackTest, MoveConstructor) {
    stack->push(1);
    stack->push(2);
    Stack<int> moved(std::move(*stack));
    EXPECT_EQ(moved.getSize(), 2);
    EXPECT_EQ(moved.getTop(), 2);
    moved.pop();
    EXPECT_EQ(moved.getTop(), 1);
    moved.pop();
    EXPECT_EQ(stack->getSize(), 0);
}

TEST_F(StackTest, ConcatOperation) {
    stack->push(1);
    stack->push(2);
    Stack<int> other;
    other.push(3);
    other.push(4);
    stack->concat(other);
    EXPECT_EQ(stack->getSize(), 4);
}

TEST_F(StackTest, ReverseOperation) {
    stack->push(1);
    stack->push(2);
    stack->push(3);
    stack->reverse();
    EXPECT_EQ(stack->getTop(), 1);
    stack->pop();
    EXPECT_EQ(stack->getTop(), 2);
    stack->pop();
    EXPECT_EQ(stack->getTop(), 3);
    stack->pop();
}

TEST_F(StackTest, GetSubStackOperation) {
    stack->push(1);
    stack->push(2);
    stack->push(3);
    stack->push(4);
    Stack<int>* subStack = stack->getSubStack(1, 3);
    EXPECT_EQ(subStack->getSize(), 2);
    EXPECT_EQ(subStack->getTop(), 3);
    subStack->pop();
    EXPECT_EQ(subStack->getTop(), 2);
    delete subStack;
}

TEST_F(StackTest, ImmutableOperations) {
    stack->push(1);
    stack->push(2);
    Stack<int>* newStack = stack->pushImmutable(3);
    EXPECT_EQ(stack->getSize(), 2);
    EXPECT_EQ(newStack->getSize(), 3);
    delete newStack;
    
    newStack = stack->popImmutable();
    EXPECT_EQ(stack->getSize(), 2);
    EXPECT_EQ(newStack->getSize(), 1);
    delete newStack;
    
    newStack = stack->reverseImmutable();
    EXPECT_EQ(stack->getSize(), 2);
    EXPECT_EQ(newStack->getSize(), 2);
    delete newStack;
}

class FlistTest : public ::testing::Test {
protected:
    Flist<int>* list;
    
    void SetUp() override {
        list = new Flist<int>();
    }
    
    void TearDown() override {
        delete list;
    }
};

TEST_F(FlistTest, InitiallyEmpty) {
    EXPECT_TRUE(list->isEmpty());
    EXPECT_EQ(list->getSize(), 0);
}

TEST_F(FlistTest, SingletonConstructor) {
    Flist<int> singletonList(5);
    EXPECT_EQ(singletonList.getSize(), 1);
    EXPECT_EQ(singletonList[0], 5);
}

TEST_F(FlistTest, CopyConstructor) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    Flist<int> copy(*list);

    EXPECT_EQ(copy.getSize(), 3);
    EXPECT_EQ(copy[0], 3);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 1);
    
    list = list->cons(4);
    EXPECT_EQ(copy.getSize(), 3);
    EXPECT_EQ(list->getSize(), 4);
    EXPECT_EQ((*list)[0], 4);
}

TEST_F(FlistTest, ConsOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    EXPECT_EQ(list->getSize(), 3);
    EXPECT_EQ((*list)[0], 3);
    EXPECT_EQ((*list)[1], 2);
    EXPECT_EQ((*list)[2], 1);
}

TEST_F(FlistTest, HeadAndLastOperations) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    EXPECT_EQ(list->head(), 3);
    EXPECT_EQ(list->last(), 1);
}

TEST_F(FlistTest, TailAndInitOperations) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    Flist<int>* tailList = list->tail();
    EXPECT_EQ(tailList->getSize(), 2);
    EXPECT_EQ((*tailList)[0], 2);
    EXPECT_EQ((*tailList)[1], 1);
    
    Flist<int>* initList = list->init();
    EXPECT_EQ(initList->getSize(), 2);
    EXPECT_EQ((*initList)[0], 3);
    EXPECT_EQ((*initList)[1], 2);
    
    delete tailList;
    delete initList;
}

TEST_F(FlistTest, UnconsAndUnsnocOperations) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    auto [head, tail] = list->uncons();
    EXPECT_EQ(head, 3);
    EXPECT_EQ(tail->getSize(), 2);
    
    auto [init, last] = list->unsnoc();
    EXPECT_EQ(last, 1);
    EXPECT_EQ(init->getSize(), 2);
}

TEST_F(FlistTest, ConcatOperation) {
    list = list->cons(2);
    list = list->cons(1);

    auto other = new Flist<int>();
    other = other->cons(4);
    other = other->cons(3);
    
    Flist<int>* result = list->concat(*other);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 2);
    EXPECT_EQ((*result)[2], 3);
    EXPECT_EQ((*result)[3], 4);
    
    delete result;
    delete other;
}

TEST_F(FlistTest, ReverseOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);

    list->reverse();
    auto reversed = Flist<int>( *list );
    EXPECT_EQ(reversed.getSize(), 3);
    EXPECT_EQ(reversed[0], 1);
    EXPECT_EQ(reversed[1], 2);
    EXPECT_EQ(reversed[2], 3);
    
}

TEST_F(FlistTest, GetSubFlistOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    list = list->cons(4);
    
    Flist<int>* subList = list->getSubFlist(1, 3);
    EXPECT_EQ(subList->getSize(), 2);
    EXPECT_EQ((*subList)[0], 3);
    EXPECT_EQ((*subList)[1], 2);
    
    delete subList;
}

TEST_F(FlistTest, MapOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    auto doubleFunc = [](const int& value) -> int { return value * 2; };
    Flist<int>* mapped = list->map(doubleFunc);
    EXPECT_EQ(mapped->getSize(), 3);
    EXPECT_EQ((*mapped)[0], 6);
    EXPECT_EQ((*mapped)[1], 4);
    EXPECT_EQ((*mapped)[2], 2);
    
    delete mapped;
}

TEST_F(FlistTest, WhereOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    list = list->cons(4);
    
    auto isEvenFunc = [](const int& value) -> bool { return value % 2 == 0; };
    Flist<int>* filtered = list->where(isEvenFunc);
    EXPECT_EQ(filtered->getSize(), 2);
    EXPECT_EQ((*filtered)[0], 4);
    EXPECT_EQ((*filtered)[1], 2);
    
    delete filtered;
}

TEST_F(FlistTest, SwapOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    Flist<int>* swapped = list->swap(0, 2);
    EXPECT_EQ(swapped->getSize(), 3);
    EXPECT_EQ((*swapped)[0], 1);
    EXPECT_EQ((*swapped)[1], 2);
    EXPECT_EQ((*swapped)[2], 3);
    
    delete swapped;
}

class StackSequenceTest : public ::testing::Test {
protected:
    StackSequence<int>* seq;
    
    void SetUp() override {
        seq = new StackSequence<int>();
    }
    
    void TearDown() override {
        delete seq;
    }
};

TEST_F(StackSequenceTest, InitiallyEmpty) {
    EXPECT_TRUE(seq->isEmpty());
    EXPECT_EQ(seq->getSize(), 0);
}

TEST_F(StackSequenceTest, AppendOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 3);
}

TEST_F(StackSequenceTest, PrependOperation) {
    seq->prepend(1);
    seq->prepend(2);
    seq->prepend(3);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(StackSequenceTest, InsertAtOperation) {
    seq->append(1);
    seq->append(3);
    seq->insertAt(2, 1);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 3);
}

TEST_F(StackSequenceTest, RemoveAtOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->removeAt(1);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 3);
}

TEST_F(StackSequenceTest, SetAtOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->setAt(4, 1);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 4);
    EXPECT_EQ((*seq)[2], 3);
}

TEST_F(StackSequenceTest, SwapOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->swap(0, 2);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(StackSequenceTest, GetSubSequenceOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->append(4);

    Sequence<int>* subSeq = seq->getSubSequence(1, 3);
    EXPECT_EQ(subSeq->getSize(), 2);
    EXPECT_EQ((*subSeq)[0], 2);
    EXPECT_EQ((*subSeq)[1], 3);
    delete subSeq;
}

TEST_F(StackSequenceTest, ConcatOperation) {
    seq->append(1);
    seq->append(2);
    auto other = new StackSequence<int>();
    other->append(3);
    other->append(4);

    Sequence<int>* result = seq->concat(*other);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 2);
    EXPECT_EQ((*result)[2], 3);
    EXPECT_EQ((*result)[3], 4);
}

TEST_F(StackSequenceTest, ImmutableOperations) {
    seq->append(1);
    seq->append(2);
    
    Sequence<int>* newSeq = seq->appendImmutable(3);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[2], 3);
    delete newSeq;
    
    newSeq = seq->prependImmutable(0);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[0], 0);
    delete newSeq;
    
    newSeq = seq->setAtImmutable(10, 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*newSeq)[1], 10);
    delete newSeq;
    
    newSeq = seq->swapImmutable(0, 1);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*newSeq)[0], 2);
    EXPECT_EQ((*newSeq)[1], 1);
    delete newSeq;
}

TEST_F(StackSequenceTest, CloneOperation) {
    seq->append(1);
    seq->append(2);
    Sequence<int>* clone = seq->clone();
    EXPECT_EQ(clone->getSize(), 2);
    EXPECT_EQ((*clone)[0], 1);
    EXPECT_EQ((*clone)[1], 2);
    seq->append(3);
    EXPECT_EQ(clone->getSize(), 2);
    delete clone;
}

TEST_F(StackSequenceTest, CopyOperation) {
    seq->append(1);
    seq->append(2);
    StackSequence<int> other;
    other.append(3);
    other.append(4);
    seq->copy(other);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 4);
}

class FlistSequenceTest : public ::testing::Test {
protected:
    FlistSequence<int>* seq;
    
    void SetUp() override {
        seq = new FlistSequence<int>();
    }
    
    void TearDown() override {
        delete seq;
    }
};

TEST_F(FlistSequenceTest, InitiallyEmpty) {
    EXPECT_TRUE(seq->isEmpty());
    EXPECT_EQ(seq->getSize(), 0);
}

TEST_F(FlistSequenceTest, AppendOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 3);
}

TEST_F(FlistSequenceTest, PrependOperation) {
    seq->prepend(1);
    seq->prepend(2);
    seq->prepend(3);
    seq->prepend(4);
    seq->prepend(5);
    seq->prepend(6);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(FlistSequenceTest, InsertAtOperation) {
    seq->append(1);
    seq->append(3);
    seq->insertAt(2, 1);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 3);
}

TEST_F(FlistSequenceTest, RemoveAtOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->removeAt(1);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 3);
}

TEST_F(FlistSequenceTest, SetAtOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->setAt(4, 1);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 4);
    EXPECT_EQ((*seq)[2], 3);
}

TEST_F(FlistSequenceTest, SwapOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->swap(0, 2);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(FlistSequenceTest, GetSubSequenceOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->append(4);
    Sequence<int>* subSeq = seq->getSubSequence(1, 3);
    EXPECT_EQ(subSeq->getSize(), 2);
    EXPECT_EQ((*subSeq)[0], 2);
    EXPECT_EQ((*subSeq)[1], 3);
    delete subSeq;
}

TEST_F(FlistSequenceTest, ConcatOperation) {
    seq->append(1);
    seq->append(2);
    FlistSequence<int> other;
    other.append(3);
    other.append(4);
    Sequence<int>* result = seq->concat(other);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 2);
    EXPECT_EQ((*result)[2], 3);
    EXPECT_EQ((*result)[3], 4);
    delete result;
}

TEST_F(FlistSequenceTest, ImmutableOperations) {
    seq->append(1);
    seq->append(2);
    
    Sequence<int>* newSeq = seq->appendImmutable(3);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[2], 3);
    delete newSeq;
    
    newSeq = seq->prependImmutable(0);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[0], 0);
    delete newSeq;
    
    newSeq = seq->setAtImmutable(10, 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*newSeq)[1], 10);
    delete newSeq;
    
    newSeq = seq->swapImmutable(0, 1);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*newSeq)[0], 2);
    EXPECT_EQ((*newSeq)[1], 1);
    delete newSeq;
}

TEST_F(FlistSequenceTest, CloneOperation) {
    seq->append(1);
    seq->append(2);
    Sequence<int>* clone = seq->clone();
    EXPECT_EQ(clone->getSize(), 2);
    EXPECT_EQ((*clone)[0], 1);
    EXPECT_EQ((*clone)[1], 2);
    seq->append(3);
    EXPECT_EQ(clone->getSize(), 2);
    delete clone;
}

TEST_F(FlistSequenceTest, CopyOperation) {
    seq->append(1);
    seq->append(2);
    FlistSequence<int> other;
    other.append(3);
    other.append(4);
    seq->copy(other);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 4);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}