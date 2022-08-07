import scala.annotation.tailrec

def factorial(n: Int) =
  @tailrec
  def loop(n: Int, acc: Int): Int =
    if n == 0 then acc
    else loop(n - 1, acc * n)
  loop(n, 1)

@main def test() =
  println(factorial(1))
