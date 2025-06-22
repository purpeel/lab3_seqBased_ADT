#include <gtest/gtest.h>
#include "../inc/tmplInc/Hanoi.hpp"
#include "../inc/tmplInc/Stack.hpp"
#include "../inc/tmplInc/Flist.hpp"
#include "../inc/tmplInc/StackSequence.hpp"
#include "../inc/tmplInc/FlistSequence.hpp"

class HanoiTest : public ::testing::Test {
protected:
    Hanoi* hanoi;
    void SetUp() override { hanoi = new Hanoi(3); }
    void TearDown() override { delete hanoi; }
};

TEST_F(HanoiTest, InitializationTest) {
    EXPECT_EQ(hanoi->getPole(1)->getSize(), 3);
    EXPECT_EQ(hanoi->getPole(2)->getSize(), 0);
    EXPECT_EQ(hanoi->getPole(3)->getSize(), 0);
}

TEST_F(HanoiTest, SolveTest) {
    hanoi->solve(3, 1, 3);
    EXPECT_EQ(hanoi->getPole(1)->getSize(), 0);
    EXPECT_EQ(hanoi->getPole(3)->getSize(), 3);
}

class StackTest : public ::testing::Test {
protected:
    Stack<int>* stack;
    void SetUp() override { stack = new Stack<int>(); }
    void TearDown() override { delete stack; }
};

TEST_F(StackTest, BasicOperations) {
    EXPECT_TRUE(stack->isEmpty());
    stack->push(1);
    stack->push(2);
    EXPECT_EQ(stack->getSize(), 2);
    EXPECT_EQ(stack->getTop(), 2);
    stack->pop();
    EXPECT_EQ(stack->getTop(), 1);
}

TEST_F(StackTest, CopyConstructor) {
    stack->push(1);
    stack->push(2);
    Stack<int> copy(*stack);
    EXPECT_EQ(copy.getSize(), 2);
    EXPECT_EQ(copy.getTop(), 2);
}

TEST_F(StackTest, ReverseOperation) {
    stack->push(1);
    stack->push(2);
    stack->push(3);
    stack->reverse();
    EXPECT_EQ(stack->getTop(), 1);
    stack->pop();
    EXPECT_EQ(stack->getTop(), 2);
}

TEST_F(StackTest, ConcatOperation) {
    stack->push(1);
    stack->push(2);
    Stack<int> other;
    other.push(3);
    other.push(4);
    Stack<int>* result = stack->concat(other);
    EXPECT_EQ(result->getSize(), 4);
}

TEST_F(StackTest, GetSubStackOperation) {
    stack->push(1);
    stack->push(2);
    stack->push(3);
    stack->push(4);
    Stack<int>* subStack = stack->getSubStack(1, 3);
    EXPECT_EQ(subStack->getSize(), 2);
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

TEST_F(StackTest, FunctionalOperations) {
    stack->push(1);
    stack->push(2);
    stack->push(3);
    
    auto doubleFunc = [](const int& value) -> int { return value * 2; };
    Stack<int>* mapped = stack->mapImmutable(doubleFunc);
    EXPECT_EQ(mapped->getSize(), 3);
    delete mapped;
    
    auto isEvenFunc = [](const int& value) -> bool { return value % 2 == 0; };
    Stack<int>* filtered = stack->whereImmutable(isEvenFunc);
    EXPECT_EQ(filtered->getSize(), 1);
    delete filtered;
    
    auto addFunc = [](const int& a, const int& b) -> int { return a + b; };
    int sum = stack->foldl(addFunc, 0);
    EXPECT_EQ(sum, 6);
}

class FlistTest : public ::testing::Test {
protected:
    Flist<int>* list;
    void SetUp() override { list = new Flist<int>(); }
    void TearDown() override { delete list; }
};

TEST_F(FlistTest, BasicOperations) {
    EXPECT_TRUE(list->isEmpty());
    list = list->cons(1);
    list = list->cons(2);
    EXPECT_EQ(list->getSize(), 2);
    EXPECT_EQ(list->head(), 2);
    EXPECT_EQ(list->last(), 1);
    EXPECT_EQ((*list)[0], 2);
    EXPECT_EQ((*list)[1], 1);
}

TEST_F(FlistTest, MapOperation) {
    list = list->cons(1);
    list = list->cons(2);
    auto doubleFunc = [](const int& value) -> int { return value * 2; };
    Flist<int>* mapped = list->map(doubleFunc);
    EXPECT_EQ((*mapped)[0], 4);
    EXPECT_EQ((*mapped)[1], 2);
    delete mapped;
}

TEST_F(FlistTest, TailAndInitOperations) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    Flist<int>* tailList = list->tail();
    EXPECT_EQ(tailList->getSize(), 2);
    EXPECT_EQ((*tailList)[0], 2);
    EXPECT_EQ((*tailList)[1], 1);
    
    EXPECT_EQ((*list)[0], 3);
    EXPECT_EQ((*list)[1], 2);
    EXPECT_EQ((*list)[2], 1);
    
    Flist<int>* initList = list->init();
    EXPECT_EQ(initList->getSize(), 2);
    EXPECT_EQ((*initList)[0], 3);
    delete initList;
}

TEST_F(FlistTest, UnconsAndUnsnocOperations) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    auto [head, tail] = list->uncons();
    EXPECT_EQ(head, 3);
    EXPECT_EQ(tail->getSize(), 2);
    EXPECT_EQ((*tail)[0], 2);
    EXPECT_EQ((*tail)[1], 1);
    
    auto [init, last] = list->unsnoc();
    EXPECT_EQ(last, 1);
    EXPECT_EQ(init->getSize(), 2);
}

TEST_F(FlistTest, SwapOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    Flist<int>* swapped = list->swap(0, 2);
    EXPECT_EQ((*swapped)[0], 1);
    EXPECT_EQ((*swapped)[2], 3);
    delete swapped;
}

TEST_F(FlistTest, ReverseOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    Flist<int>* reversed = list->reverse();
    EXPECT_EQ((*reversed)[0], 1);
    EXPECT_EQ((*reversed)[1], 2);
    EXPECT_EQ((*reversed)[2], 3);
    delete reversed;
}

TEST_F(FlistTest, ConcatOperation) {
    list = list->cons(1);
    list = list->cons(2);
    
    Flist<int> other;
    Flist<int>* otherList = other.cons(3)->cons(4);

    Flist<int>* result = list->concat(*otherList);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[0], 2);
    EXPECT_EQ((*result)[2], 4);
    delete result;
}

TEST_F(FlistTest, WhereOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    list = list->cons(4);
    
    auto isEvenFunc = [](const int& value) -> bool { return value % 2 == 0; };
    Flist<int>* filtered = list->where(isEvenFunc);
    EXPECT_EQ(filtered->getSize(), 2);
    delete filtered;
}

TEST_F(FlistTest, GetSubListOperation) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    list = list->cons(4);
    
    Flist<int>* subList = list->getSubList(1, 3);
    EXPECT_EQ(subList->getSize(), 2);
    delete subList;
}

TEST_F(FlistTest, FoldOperations) {
    list = list->cons(1);
    list = list->cons(2);
    list = list->cons(3);
    
    auto addFunc = [](const int& a, const int& b) -> int { return a + b; };
    int sum = list->foldl(addFunc, 0);
    EXPECT_EQ(sum, 6);
    
    int sumR = list->foldr(addFunc, 0);
    EXPECT_EQ(sumR, 6);
}

class StackSequenceTest : public ::testing::Test {
protected:
    StackSequence<int>* seq;
    void SetUp() override { seq = new StackSequence<int>(); }
    void TearDown() override { delete seq; }
};

TEST_F(StackSequenceTest, BasicOperations) {
    EXPECT_TRUE(seq->isEmpty());
    seq->append(1);
    seq->append(2);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);
}

TEST_F(StackSequenceTest, InsertRemoveOperations) {
    seq->append(1);
    seq->append(3);
    seq->insertAt(2, 1);
    EXPECT_EQ((*seq)[1], 2);
    seq->removeAt(1);
    EXPECT_EQ((*seq)[1], 3);
}

TEST_F(StackSequenceTest, ImmutableOperations) {
    seq->append(1);
    seq->append(2);
    Sequence<int>* newSeq = seq->appendImmutable(3);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    delete newSeq;
}

TEST_F(StackSequenceTest, CloneOperation) {
    seq->append(1);
    seq->append(2);
    Sequence<int>* clone = seq->clone();
    EXPECT_EQ(clone->getSize(), 2);
    EXPECT_EQ((*clone)[0], 1);
    delete clone;
}

TEST_F(StackSequenceTest, SwapOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->swap(0, 2);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(StackSequenceTest, PrependOperation) {
    seq->prepend(1);
    seq->prepend(2);
    seq->prepend(3);
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(StackSequenceTest, SetAtOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->setAt(4, 1);
    EXPECT_EQ((*seq)[1], 4);
}

TEST_F(StackSequenceTest, GetSubSequenceOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->append(4);
    Sequence<int>* subSeq = seq->getSubSequence(1, 3);
    EXPECT_EQ(subSeq->getSize(), 2);
    EXPECT_EQ((*subSeq)[0], 2);
    delete subSeq;
}

TEST_F(StackSequenceTest, ConcatOperation) {
    seq->append(1);
    seq->append(2);
    StackSequence<int> other;
    other.append(3);
    other.append(4);
    Sequence<int>* result = seq->concat(other);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[2], 3);
}

TEST_F(StackSequenceTest, FunctionalOperations) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    
    auto doubleFunc = [](const int& value) -> int { return value * 2; };
    seq->map(doubleFunc);
    EXPECT_EQ((*seq)[0], 2);
    EXPECT_EQ((*seq)[1], 4);
    
    seq->clear();
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->append(4);
    
    auto isEvenFunc = [](const int& value) -> bool { return value % 2 == 0; };
    seq->where(isEvenFunc);
    EXPECT_EQ(seq->getSize(), 2);
    
    auto addFunc = [](const int& a, const int& b) -> int { return a + b; };
    int sum = seq->foldl(addFunc, 0);
    EXPECT_GT(sum, 0);
}

TEST_F(StackSequenceTest, AllImmutableOperations) {
    seq->append(1);
    seq->append(2);
    
    Sequence<int>* newSeq = seq->insertAtImmutable(3, 1);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    delete newSeq;
    
    newSeq = seq->removeAtImmutable(0);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 1);
    delete newSeq;
    
    newSeq = seq->setAtImmutable(10, 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*newSeq)[1], 10);
    delete newSeq;
    
    newSeq = seq->swapImmutable(0, 1);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*newSeq)[0], 2);
    delete newSeq;
    
    StackSequence<int> other;
    other.append(3);
    other.append(4);
    newSeq = seq->concatImmutable(other);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 4);
    delete newSeq;
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
}

class FlistSequenceTest : public ::testing::Test {
protected:
    FlistSequence<int>* seq;
    void SetUp() override { seq = new FlistSequence<int>(); }
    void TearDown() override { delete seq; }
};

TEST_F(FlistSequenceTest, BasicOperations) {
    EXPECT_TRUE(seq->isEmpty());
    seq->append(1);
    seq->append(2);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);
}

TEST_F(FlistSequenceTest, InsertRemoveOperations) {
    seq->append(1);
    seq->append(3);
    seq->insertAt(2, 1);
    EXPECT_EQ((*seq)[1], 2);
    seq->removeAt(1);
    EXPECT_EQ((*seq)[1], 3);
}

TEST_F(FlistSequenceTest, ImmutableOperations) {
    seq->append(1);
    seq->append(2);
    Sequence<int>* newSeq = seq->appendImmutable(3);

    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);

    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[0], 1);
    EXPECT_EQ((*newSeq)[1], 2);
    EXPECT_EQ((*newSeq)[2], 3);
    delete newSeq;
}

TEST_F(FlistSequenceTest, CloneOperation) {
    seq->append(1);
    seq->append(2);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*seq)[1], 2);

    Sequence<int>* clone = seq->clone();
    EXPECT_EQ(clone->getSize(), 2);
    EXPECT_EQ((*clone)[0], 1);
    EXPECT_EQ((*clone)[1], 2);

    delete clone;
}

TEST_F(FlistSequenceTest, SwapOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    EXPECT_EQ((*seq)[2], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[0], 1);
    
    seq->swap(0, 2);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(FlistSequenceTest, PrependOperation) {
    seq->prepend(1);
    seq->prepend(2);
    seq->prepend(3);
    
    EXPECT_EQ(seq->getSize(), 3);
    EXPECT_EQ((*seq)[0], 3);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*seq)[2], 1);
}

TEST_F(FlistSequenceTest, SetAtOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->setAt(4, 1);
    EXPECT_EQ((*seq)[1], 4);
}

TEST_F(FlistSequenceTest, GetSubSequenceOperation) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->append(4);
    Sequence<int>* subSeq = seq->getSubSequence(1, 3);
    EXPECT_EQ(subSeq->getSize(), 2);
    EXPECT_EQ((*subSeq)[0], 2);
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
    EXPECT_EQ((*result)[2], 3);
}

TEST_F(FlistSequenceTest, FunctionalOperations) {
    seq->append(1);
    seq->append(2);
    seq->append(3);
    
    auto doubleFunc = [](const int& value) -> int { return value * 2; };
    seq->map(doubleFunc);
    EXPECT_EQ((*seq)[0], 2);
    EXPECT_EQ((*seq)[1], 4);
    
    seq->clear();
    seq->append(1);
    seq->append(2);
    seq->append(3);
    seq->append(4);
    
    auto isEvenFunc = [](const int& value) -> bool { return value % 2 == 0; };
    seq->where(isEvenFunc);
    EXPECT_EQ(seq->getSize(), 2);
    
    auto addFunc = [](const int& a, const int& b) -> int { return a + b; };
    int sum = seq->foldl(addFunc, 0);
    EXPECT_GT(sum, 0);
}

TEST_F(FlistSequenceTest, AllImmutableOperations) {
    seq->append(1);
    seq->append(2);
    
    Sequence<int>* newSeq = seq->insertAtImmutable(3, 1);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    delete newSeq;
    
    newSeq = seq->removeAtImmutable(0);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 1);
    delete newSeq;
    
    newSeq = seq->setAtImmutable(10, 1);
    EXPECT_EQ((*seq)[1], 2);
    EXPECT_EQ((*newSeq)[1], 10);
    delete newSeq;
    
    newSeq = seq->swapImmutable(0, 1);
    EXPECT_EQ((*seq)[0], 1);
    EXPECT_EQ((*newSeq)[0], 2);
    delete newSeq;
    
    FlistSequence<int> other;
    other.append(3);
    other.append(4);
    newSeq = seq->concatImmutable(other);
    EXPECT_EQ(seq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 4);
    delete newSeq;
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
}

class PolymorphicSequenceTest : public ::testing::Test {
protected:
    Sequence<int>* stackSeq;
    Sequence<int>* flistSeq;
    
    void SetUp() override {
        stackSeq = new StackSequence<int>();
        flistSeq = new FlistSequence<int>();
    }
    
    void TearDown() override {
        delete stackSeq;
        delete flistSeq;
    }
};

TEST_F(PolymorphicSequenceTest, PolymorphicBasicOperations) {
    stackSeq->append(1);
    stackSeq->append(2);
    flistSeq->append(1);
    flistSeq->append(2);
    
    EXPECT_EQ(stackSeq->getSize(), 2);
    EXPECT_EQ(flistSeq->getSize(), 2);
    EXPECT_EQ((*stackSeq)[0], 1);
    EXPECT_EQ((*flistSeq)[0], 1);
}

TEST_F(PolymorphicSequenceTest, PolymorphicCloneAndCopy) {
    stackSeq->append(1);
    stackSeq->append(2);
    
    Sequence<int>* clone = stackSeq->clone();
    EXPECT_EQ(clone->getSize(), 2);
    EXPECT_EQ((*clone)[0], 1);
    
    flistSeq->copy(*stackSeq);
    EXPECT_EQ(flistSeq->getSize(), 2);
    EXPECT_EQ((*flistSeq)[0], 1);
    
    delete clone;
}

TEST_F(PolymorphicSequenceTest, PolymorphicConcat) {
    stackSeq->append(1);
    stackSeq->append(2);
    flistSeq->append(3);
    flistSeq->append(4);
    
    Sequence<int>* result = stackSeq->concat(*flistSeq);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[2], 3);
}

TEST_F(PolymorphicSequenceTest, PolymorphicSubSequence) {
    stackSeq->append(1);
    stackSeq->append(2);
    stackSeq->append(3);
    stackSeq->append(4);
    
    Sequence<int>* subSeq = stackSeq->getSubSequence(1, 3);
    EXPECT_EQ(subSeq->getSize(), 2);
    EXPECT_EQ((*subSeq)[0], 2);
    EXPECT_EQ((*subSeq)[1], 3);
    delete subSeq;
}

TEST_F(PolymorphicSequenceTest, PolymorphicImmutableOperations) {
    stackSeq->append(1);
    stackSeq->append(2);
    
    Sequence<int>* newSeq = stackSeq->appendImmutable(3);
    EXPECT_EQ(stackSeq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[2], 3);
    delete newSeq;
    
    newSeq = stackSeq->prependImmutable(0);
    EXPECT_EQ(stackSeq->getSize(), 2);
    EXPECT_EQ(newSeq->getSize(), 3);
    EXPECT_EQ((*newSeq)[0], 0);
    delete newSeq;
}

TEST_F(PolymorphicSequenceTest, PolymorphicFunctionalOperations) {
    stackSeq->append(1);
    stackSeq->append(2);
    stackSeq->append(3);
    
    auto doubleFunc = [](const int& value) -> int { return value * 2; };
    Sequence<int>* mapped = stackSeq->mapImmutable(doubleFunc);
    EXPECT_EQ((*mapped)[0], 2);
    EXPECT_EQ((*mapped)[1], 4);
    EXPECT_EQ((*mapped)[2], 6);
    delete mapped;
    
    auto isEvenFunc = [](const int& value) -> bool { return value % 2 == 0; };
    Sequence<int>* filtered = stackSeq->whereImmutable(isEvenFunc);
    EXPECT_EQ(filtered->getSize(), 1);
    EXPECT_EQ((*filtered)[0], 2);
    delete filtered;
    
    auto addFunc = [](const int& a, const int& b) -> int { return a + b; };
    int sum = stackSeq->foldl(addFunc, 0);
    EXPECT_EQ(sum, 6);
    
    int sumR = stackSeq->foldr(addFunc, 0);
    EXPECT_EQ(sumR, 6);
}

TEST_F(PolymorphicSequenceTest, PolymorphicSwapOperations) {
    stackSeq->append(1);
    stackSeq->append(2);
    stackSeq->append(3);
    
    stackSeq->swap(0, 2);
    EXPECT_EQ((*stackSeq)[0], 3);
    EXPECT_EQ((*stackSeq)[2], 1);
    
    Sequence<int>* swapped = stackSeq->swapImmutable(0, 2);
    EXPECT_EQ((*stackSeq)[0], 3);
    EXPECT_EQ((*swapped)[0], 1);
    delete swapped;
}

TEST_F(PolymorphicSequenceTest, PolymorphicAllOperations) {
    stackSeq->append(1);
    stackSeq->prepend(0);
    stackSeq->insertAt(2, 2);
    EXPECT_EQ(stackSeq->getSize(), 3);
    EXPECT_EQ((*stackSeq)[0], 0);
    EXPECT_EQ((*stackSeq)[2], 2);
    
    stackSeq->setAt(10, 1);
    EXPECT_EQ((*stackSeq)[1], 10);
    
    stackSeq->removeAt(1);
    EXPECT_EQ(stackSeq->getSize(), 2);
}

TEST_F(PolymorphicSequenceTest, CrossTypeCompatibility) {
    StackSequence<int> stack;
    FlistSequence<int> flist;
    
    stack.append(1);
    stack.append(2);
    flist.append(3);
    flist.append(4);
    
    Sequence<int>* result = stack.concatImmutable(flist);
    EXPECT_EQ(result->getSize(), 4);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[2], 3);
    
    flist.copy(stack);
    EXPECT_EQ(flist.getSize(), 2);
    EXPECT_EQ(flist[0], 1);
    EXPECT_EQ(flist[1], 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}