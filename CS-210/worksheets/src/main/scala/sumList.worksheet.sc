import scala.annotation.tailrec

val testList = List(1, 2, 3)

testList.head
testList.isEmpty
testList.tail
testList.tail.head
testList ++ List(4, 6)

def sumList(ls: List[Int]) =
  @tailrec
  def loop(ls: List[Int], acc: Int): Int =
    if ls.isEmpty then acc
    else loop(ls.tail, acc + ls.head)
  loop(ls, 0)

sumList(List())
sumList(List(1, 5))
sumList(List(6, 7, 8, 9))
