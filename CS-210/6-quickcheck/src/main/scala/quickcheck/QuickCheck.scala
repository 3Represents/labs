package quickcheck

import org.scalacheck.*
import Arbitrary.*
import Gen.*
import Prop.forAll

abstract class QuickCheckHeap extends Properties("Heap") with IntHeap:
  lazy val genHeap: Gen[H] =
    for
      x <- arbitrary[A]
      h <- oneOf(const(empty), genHeap)
    yield
      insert(x, h)
      
  given Arbitrary[H] = Arbitrary(genHeap)

  property("gen1") = forAll { (h: H) =>
    val m = if isEmpty(h) then 0 else findMin(h)
    findMin(insert(m, h)) == m
  }

  property("minOf2") = forAll { (x: A, y: A) =>
    val min = if x < y then x else y
    findMin(insert(y, insert(x, empty))) == min
  }

  property("del") = forAll { (x: A) =>
    isEmpty(deleteMin(insert(x, empty)))
  }

  property("sort") = forAll { (h: H) =>
    def isSorted(subH: H, acc: List[A]): Boolean = isEmpty(subH) match
      case true => true
      case false => 
        val newMax = findMin(subH)
        acc match
        case Nil =>
          isSorted(deleteMin(subH), newMax :: acc)
        case max :: xs =>
          if newMax < max then false
          else isSorted(deleteMin(subH), newMax :: acc)
    
    isSorted(h, Nil)
  }

  property("meld") = forAll { (h1: H, h2: H) =>
    def isEqual(h1: H, h2: H): Boolean = (isEmpty(h1), isEmpty(h2)) match
      case (true, true) => true
      case _ =>
        if findMin(h1) == findMin(h2) then isEqual(deleteMin(h1), deleteMin(h2))
        else false

    isEqual(
      meld(h1, h2),
      meld(deleteMin(h1), insert(findMin(h1), h2))
    )
  }