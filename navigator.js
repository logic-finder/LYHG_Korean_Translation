const content = (title, link) => ({title, link});
const toc = [
   content("원본 주소", "https://learnyouahaskell.com/"),
   content("이 번역본 깃헙 레포지토리", "https://github.com/logic-finder/LYHG_korean_translation"),
   content("자주 나오는 질문들", "faq"),
   content("1장. 소개", "introduction"),
   content("2장. 시작하기", "starting-out"),
   content("3장. 타입과 타입클래스", "types-and-typeclasses"),
   content("4장. 함수에서의 구문", "syntax-in-functions"),
   content("5장. 재귀", "recursion"),
   content("6장. 고차함수", "higher-order-functions"),
   content("7장. 모듈", "modules"),
   content("8장. 사용자 정의 타입과 타입클래스", "making-our-own-types-and-typeclasses"),
   content("9장. 입출력", "input-and-output"),
   content("10장. 함수적으로 문제 해결하기", "functionally-solving-problems"),
   content("11장. 함자, 응용 함자, 그리고 모노이드", "functors-applicative-functors-and-monoids"),
   content("12장. 모나드", "a-fistful-of-monads"),
   content("13장. 모나드 심화", "/or-a-few-monads-more"),
   content("14장. 지퍼", "zippers"),
];
const list = document.querySelector("#navigator > ul");

function constructTableOfContents(currentLink) {
   for (let i = 0; i < toc.length; i++) {
      const li = document.createElement("li");

      if (toc[i].link == currentLink) {
         const b = document.createElement("b");
         b.textContent = ">> " + toc[i].title;
         li.appendChild(b);
      }
      else {
         const a = document.createElement("a");
         a.textContent = toc[i].title;
         a.href = toc[i].link;
         li.appendChild(a);
      }
      list.appendChild(li);
   }
}