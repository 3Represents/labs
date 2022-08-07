package recfun

object RecFun extends RecFunInterface:

  def main(args: Array[String]): Unit =
    println("Pascal's Triangle")
    for row <- 0 to 10 do
      for col <- 0 to row do
        print(s"${pascal(col, row)} ")
      println()

  /**
   * Exercise 1
   */
  def pascal(c: Int, r: Int): Int =
    if (c == 0) || (c == r) then
      1
    else
      pascal(c-1, r-1) + pascal(c, r-1)

  /**
   * Exercise 2
   */
  def balance(chars: List[Char]): Boolean =
    def balance(chars: List[Char], nOpen: Int): Boolean =
      // nOpen: Number of open parentheses to balance
      if chars.isEmpty then
        if nOpen == 0 then true else false
      else if nOpen < 0 then
        false
      else
        if chars.head == '(' then
          balance(chars.tail, nOpen+1)
        else if chars.head == ')' then
          balance(chars.tail, nOpen-1)
        else
          balance(chars.tail, nOpen)

    balance(chars, 0)

  /**
   * Exercise 3
   */
  def countChange(money: Int, coins: List[Int]): Int =
    if (money <= 0) || (coins.isEmpty) then
      0
    else if money == coins.head then
      1 + countChange(money, coins.tail)
    else if money < coins.head then
      countChange(money, coins.tail)
    else
      countChange(money, coins.tail) + countChange(money-coins.head, coins)
