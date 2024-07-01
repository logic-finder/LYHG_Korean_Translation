const content = (title, link) => ({ title, link });
const navMiscData = [
   content("원본 바로가기", "https://learnyouahaskell.com"),
   content("번역본 레포지토리 바로가기", "https://github.com/logic-finder/LYHG_korean_translation")
];
const navIntroData = [
   content("자주 나오는 질문들", "faq"),
   content("소개", "introduction")
];
const navMainData = [
   content("시작하기", "starting-out"),
   content("타입과 타입클래스", "types-and-typeclasses"),
   content("함수에서의 구문", "syntax-in-functions"),
   content("재귀", "recursion"),
   content("고차함수", "higher-order-functions"),
   content("모듈", "modules"),
   content("사용자 정의 타입과\n타입클래스", "making-our-own-types-and-typeclasses"),
   content("입출력", "input-and-output"),
   content("함수적으로 문제 해결하기", "functionally-solving-problems"),
   content("함자, 응용 함자,\n그리고 모노이드", "functors-applicative-functors-and-monoids"),
   content("모나드", "a-fistful-of-monads"),
   content("모나드 심화", "or-a-few-monads-more"),
   content("지퍼", "zippers"),
];

/**
 * @brief makes the table of contents in order to fill the sidebar.
 * 
 * @param currentDocTitle { string } the title of the current chapter
 */
function constructTOC(currentDocTitle) {
   const miscList = document.querySelector("#navigator > #nav-misc");
   const introList = document.querySelector("#navigator > #nav-intro");
   const mainList = document.querySelector("#navigator > #nav-main");

   fillList_Li(miscList, navMiscData);
   fillList_Li(introList, navIntroData);
   fillList_Div(mainList, navMainData);

   const cdInfo = locateCurrentDocument([navIntroData, navMainData], currentDocTitle);
   if (!cdInfo.found)
      console.error(`navigator: couldn't find ${currentDocTitle}.`);

   if (cdInfo.nthArr === 0) {
      const listItem = introList.children[cdInfo.nthElem];
      markItem_Li(listItem);
      constructSubTOC_Li(listItem);
      prefixNumber2Hx_Rom(cdInfo.nthElem);
   }
   else if (cdInfo.nthArr === 1) {
      const listItem = mainList.children[cdInfo.nthElem];
      markItem_Div(listItem);
      constructSubTOC_Div(listItem);
      prefixNumber2Hx_Dec(cdInfo.nthElem);
   }
}

/**
 * @brief fills a list which takes `<li>` as its items.
 * 
 * @param list { HTMLOListElement | HTMLUListElement } a list to be filled
 * @param data { { title, link }[] } an array which consists of `content()` elements
 */
function fillList_Li(list, data) {
   for (let i = 0; i < data.length; i++) {
      const item = document.createElement("li");
      const a = document.createElement("a");
      
      a.innerText = data[i].title;
      a.href = data[i].link;
      item.appendChild(a);
      list.appendChild(item);
   }
}

/**
 * @brief fills a list which takes `<div>` as its items.
 * 
 * @param list { HTMLOListElement | HTMLUListElement } a list to be filled
 * @param data { { title, link }[] } an array which consists of `content()` elements
 */
function fillList_Div(list, data) {
   for (let i = 0; i < data.length; i++) {
      const item = document.createElement("div");
      const numberingP = document.createElement("p");
      const titleP = document.createElement("p");
      const a = document.createElement("a");

      item.className = "main-list-item";
      
      numberingP.textContent = `제${i}장.`;
      item.appendChild(numberingP);

      a.innerText = data[i].title;
      a.href = data[i].link;
      titleP.appendChild(a);
      item.appendChild(titleP);

      list.appendChild(item);
   }
}

/**
 * @brief tries to find an array to which a specific element belongs and the position inside of the array.
 * 
 * @param haystacks { { title: string, link: string }[][] } an array of arrays
 * @param needle { string } the name of a specific element
 */
function locateCurrentDocument(haystacks, needle) {
   let location = {
      found: false,
      nthArr: null,
      nthElem: null
   };

   for (let haystack = 0; haystack < haystacks.length; haystack++) {
      for (let hay = 0; hay < haystacks[haystack].length; hay++) {
         if (haystacks[haystack][hay].link === needle) {
            location.found = true;
            location.nthArr = haystack;
            location.nthElem = hay;
            break;
         }
      }
      if (location.found)
         break;
   }

   return location;
}

/**
 * @brief marks a current item in a list having `<li>` items.
 * 
 * @param listItem { HTMLLIElement } an item to mark
 */
function markItem_Li(listItem) {
   const b = document.createElement("b");
   const existingA = listItem.children[0];

   b.innerText = `${listItem.innerText}\n(현재 위치)`;
   listItem.insertBefore(b, existingA);
   existingA.remove();
}

/**
 * @brief makes a sub-list inside of a list having `<li>` items.
 * 
 * @param listItem { HTMLLIElement } an item which will have a sub-list inside of it
 */
function constructSubTOC_Li(listItem) {
   const sections = document.querySelectorAll("div.plain-text > h2:first-child");

   if (sections.length == 0)
      return;

   for (let i = 0; i < sections.length; i++)
      sections[i].id = `sec-${i}`;

   const subList = document.createElement("ol");

   subList.id = "nav-sub";
   for (let i = 0; i < sections.length; i++) {
      const li = document.createElement("li");
      const a = document.createElement("a");

      a.innerText = sections[i].innerText;
      a.href = `#${sections[i].id}`;
      li.appendChild(a);
      subList.appendChild(li);
   }
   listItem.appendChild(subList);
}

/**
 * @brief numbers to `<h1>` and `<h2>` using the uppercase Roman numerals.
 * 
 * @param idx { number } a number to be used when numbering
 */
function prefixNumber2Hx_Rom(idx) {
   const title = document.querySelector("div.plain-text > h1:first-child");
   const sections = document.querySelectorAll("div.plain-text > h2:first-child");
   const prefix = idx === 0 ? "Ⅰ" : "Ⅱ";

   title.innerText = `${prefix}. ${title.innerText}`;
   for (let i = 0; i < sections.length; i++)
      sections[i].innerText = `${prefix}.${i + 1}. ${sections[i].innerText}`;
}

/**
 * @brief marks a current item in a list having `<div>` items.
 * 
 * @param listItem { HTMLDivElement } an item to mark
 */
function markItem_Div(listItem) {
   const b = document.createElement("b");
   const existingP = listItem.children[1];
   const existingA = existingP.children[0];

   b.innerText = `${existingA.innerText}\n(현재 위치)`;
   existingP.insertBefore(b, existingA);
   existingA.remove();
}

/**
 * @brief makes a sub-list inside of a list having `<div>` items.
 * 
 * @param listItem { HTMLDivElement } an item which will have a sub-list inside of it
 */
function constructSubTOC_Div(listItem) {
   const sections = document.querySelectorAll("div.plain-text > h2:first-child");

   if (sections.length == 0)
      return;

   for (let i = 0; i < sections.length; i++)
      sections[i].id = `sec-${i}`;

   const subList = document.createElement("div");

   subList.id = "nav-sub";
   for (let i = 0; i < sections.length; i++) {
      const item = document.createElement("div");
      const numberingP = document.createElement("p");
      const titleP = document.createElement("p");
      const a = document.createElement("a");

      numberingP.textContent = `제${i}절.`;
      item.appendChild(numberingP);

      a.innerText = sections[i].innerText;
      a.href = `#${sections[i].id}`;
      titleP.appendChild(a);
      item.appendChild(titleP);

      subList.appendChild(item);
   }
   listItem.appendChild(subList);
}

/**
 * @brief numbers to `<h1>` and `<h2>` using the decimal numerals.
 * 
 * @param idx { number } a number to be used when numbering
 */
function prefixNumber2Hx_Dec(idx) {
   const title = document.querySelector("div.plain-text > h1:first-child");
   const sections = document.querySelectorAll("div.plain-text > h2:first-child");
   const prefix = idx + 1;

   title.innerText = `${prefix}. ${title.innerText}`;
   for (let i = 0; i < sections.length; i++)
      sections[i].innerText = `${prefix}.${i + 1}. ${sections[i].innerText}`; 
}