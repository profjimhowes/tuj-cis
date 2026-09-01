Charles Antony Richard
"Tony" Hoare
1934 - 2026
creator of quicksort
1980 Turing Award

## On implementing ALGOL 60
> The first principle was security: The principle that every syntactically incorrect program should be rejected by the compiler and that every syntactically correct program should give a result or an error message that was predictable and comprehensible in terms of the source language program itself. Thus no core dumps should ever be necessary. It was logically impossible for any source language program to cause the computer to run wild, either at compile time or at run time. A consequence of this principle is that every occurrence of every subscript of every subscripted variable was on every occasion checked at run time against both the upper and the lower declared bounds of the array. Many years later we asked our customers whether they wished us to provide an option to switch off these checks in the interests of efficiency on production runs. Unanimously, they urged us not to--they already knew how frequently subscript errors occur on production runs where failure to detect them could be disastrous. I note with fear and horror that even in 1980 language designers and users have not learned this lesson. **In any respectable branch of engineering, failure to observe such elementary precautions would have long been against the law.**

## On performance
> If as a result of care taken in implementation the available hardware remains more powerful than may seem necessary for a particular application, the applications programmer can nearly always take advantage of the extra capacity to increase the quality of his program, its simplicity, its ruggedness, and its reliability.

## On safety over succinctness
> I was eventually persuaded of the need to design programming notations so as to maximize the number of errors which cannot be made, or if made, can be reliably detected at compile time. Perhaps this would make the text of programs longer. Never mind! [...] The way to shorten programs is to use procedures, not to omit vital declarative information.

## On the failure of a large complex project
The Elliott 503 Mark II was an ambitious software project, slated for release in 1965, that ultimately failed due to a scope that exceeded hardware capabilities of the time. Tony was responsible for the project and even after it was canceled, he was directed to salvage something usable from the work.

> Of course, the company did everything they could to help me. They took away my responsibility for hardware design and reduced the size of my programming teams. Each of my managers explained carefully his own theory of what had gone wrong and all the theories were different. At last, there breezed into my office the most senior manager of all, a general manager of our parent company, Andrew St. Johnston. I was surprised that he had even heard of me. "You know what went wrong?" he shouted--he always shouted--**"You let your programmers do things which you yourself do not understand."** I stared in astonishment. He was obviously out of touch with present-day realities. How could one person ever understand the whole of a modern software product like the Elliott 503 Mark II software system?
> I realized later that he was absolutely right; he had diagnosed the true cause of the problem and he had planted the seed of its later solution.

> You know, you shouldn't trust us intelligent programmers. We can think up such good arguments for convincing ourselves and each other of the utterly absurd. Especially don't believe us when we promise to repeat an earlier success, only bigger and better next time.

## On complexity in software design
> I conclude that there are two ways of constructing a software design: One way is to make it so simple that there are obviously no deficiencies and the other way is to make it so complicated that there are no obvious deficiencies.
> The first method is far more difficult. It demands the same skill, devotion, insight, and even inspiration as the discovery of the simple physical laws which underlie the complex phenomena of nature. It also requires a willingness to accept objectives which are limited by physical, logical, and technological constraints, and to accept a compromise when conflicting objectives cannot be met. **No committee will ever do this until it is too late.**

> A feature which is omitted can always be added later, when its design and its implications are well understood. A feature which is included before it is fully understood can never be removed later.

> Programmers are always surrounded by complexity; we cannot avoid it. Our applications are complex because we are ambitious to use our computers in ever more sophisticated ways. Programming is complex because of the large number of conflicting objectives for each of our programming projects. If our basic tool, the language in which we design and code our programs, is also complicated, the language itself becomes part of the problem rather than part of its solution.

## On reliability
> Almost anything in software can be implemented, sold, and even used given enough determination. There is nothing a mere scientist can say that will stand against the flood of a hundred million dollars. But there is one quality that cannot be purchased in this way--and that is reliability. The price of reliability is the pursuit of the utmost simplicity. **It is a price which the very rich find most hard to pay.**
