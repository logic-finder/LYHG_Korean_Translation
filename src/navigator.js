const content = (title, link) => ({ title, link });
const navMiscData = [
   content("원본 바로가기", "https://learnyouahaskell.com"),
   content("번역본 레포지토리 바로가기", "https://github.com/logic-finder/LYHG_korean_translation")
];
const navIntroData = [
   content("자주 나오는 질문들", "faq.html")
];
const navMainData = [
   content("소개", "introduction.html"),
   content("시작하기", "starting-out.html"),
   content("타입과 타입클래스", "types-and-typeclasses.html"),
   content("함수에서의 구문", "syntax-in-functions.html"),
   content("재귀", "recursion.html"),
   content("고차함수", "higher-order-functions.html"),
   content("모듈", "modules.html"),
   content("사용자 정의 타입과\n타입클래스", "making-our-own-types-and-typeclasses.html"),
   content("입출력", "input-and-output.html"),
   content("함수적으로 문제 해결하기", "functionally-solving-problems.html"),
   content("함자, 응용 함자,\n그리고 모노이드", "functors-applicative-functors-and-monoids.html"),
   content("모나드", "a-fistful-of-monads.html"),
   content("모나드 심화", "or-a-few-monads-more.html"),
   content("지퍼", "zippers.html"),
];

/**
 * @brief fills the sidebar, sets the title of a document, adds a chapter number and a section number to headings.
 * @param {string} title - the title of this document
 */
function tailorDocument(title) {
   const { found,
           nthArr,
           nthElem } = locateCurrentDocument([navIntroData, navMainData], title);
   if (!found) {
      console.error(`navigator: couldn't find ${title}.`);
      return;
   }

   constructTOC(nthArr, nthElem);
   if (nthArr === 0) {
      const prefix = getUppercaseRomanNumeral(nthElem);
      prefixNumber2Hx(prefix);
      setTitle(prefix, navIntroData[nthElem].title);
   }
   else if (nthArr === 1) {
      const prefix = nthElem + 1;
      prefixNumber2Hx(prefix);
      setTitle(prefix, navMainData[nthElem].title);
   }
}

/**
 * @typedef {Object} LocateCurrentDocumentReturn
 * @property {boolean} found - whether the item in want have been found
 * @property {number} nthArr - if so, in which array it is
 * @property {number} nthElem - if so, where it is
 */

/**
 * @brief tries to find an array to which a specific element belongs and the position inside of the array.
 * @param {{title: string, link: string}[][]} haystacks - an array of arrays
 * @param {string} needle - the name of a specific element
 * @return {LocateCurrentDocumentReturn}
 */
function locateCurrentDocument(haystacks, needle) {
   let location = {
      found: false,
      nthArr: null,
      nthElem: null
   };

   for (let haystack = 0; haystack < haystacks.length; haystack++) {
      for (let hay = 0; hay < haystacks[haystack].length; hay++)
         if (haystacks[haystack][hay].link === needle) {
            location.found = true;
            location.nthArr = haystack;
            location.nthElem = hay;
            break;
         }
      if (location.found)
         break;
   }

   return location;
}

/**
 * @brief makes the table of contents in order to fill the sidebar.
 * @param {number} nthArr - a number representing a specific array
 * @param {number} nthElem - a number representing an index of a specific element
 */
function constructTOC(nthArr, nthElem) {
   const miscList = document.querySelector("#navigator > #nav-misc");
   const introList = document.querySelector("#navigator > #nav-intro");
   const mainList = document.querySelector("#navigator > #nav-main");

   fillList_Li(miscList, navMiscData, "_blank");
   fillList_Li(introList, navIntroData);
   fillList_Div(mainList, navMainData);

   if (nthArr === 0) {
      const listItem = introList.children[nthElem];
      markItem_Li(listItem);
      constructSubTOC_Li(listItem);
   }
   else if (nthArr === 1) {
      const listItem = mainList.children[nthElem];
      markItem_Div(listItem);
      constructSubTOC_Div(listItem);
   }
}

/**
 * @brief fills a list which takes `<li>` as its items.
 * @param {HTMLOListElement|HTMLUListElement} list - a list to be filled
 * @param {{ title, link }[]} data - an array which consists of `content()` elements
 * @param {string} [target="_self"] - the target attribute of `<a>`
 */
function fillList_Li(list, data, target="_self") {
   for (let i = 0; i < data.length; i++) {
      const item = document.createElement("li");
      const a = document.createElement("a");
      
      a.innerText = data[i].title;
      a.href = data[i].link;
      a.target = target;
      item.appendChild(a);
      list.appendChild(item);
   }
}

/**
 * @brief fills a list which takes `<div>` as its items.
 * @param {HTMLOListElement|HTMLUListElement} list - a list to be filled
 * @param {{title, link }[]} data - an array which consists of `content()` elements
 */
function fillList_Div(list, data) {
   for (let i = 0; i < data.length; i++) {
      const item = document.createElement("div");
      const numberingP = document.createElement("p");
      const titleP = document.createElement("p");
      const a = document.createElement("a");

      item.className = "main-list-item";
      
      numberingP.textContent = `제${i + 1}장.`;
      item.appendChild(numberingP);

      a.innerText = data[i].title;
      a.href = data[i].link;
      titleP.appendChild(a);
      item.appendChild(titleP);

      list.appendChild(item);
   }
}

/**
 * @brief marks a current item in a list having `<li>` items.
 * @param {HTMLLIElement} listItem - an item to mark
 */
function markItem_Li(listItem) {
   const b = document.createElement("b");
   const existingA = listItem.children[0];

   b.innerText = `${listItem.innerText} (현재 위치)`;
   listItem.insertBefore(b, existingA);
   existingA.remove();
}

/**
 * @brief makes a sub-list inside of a list having `<li>` items.
 * @param {HTMLLIElement} listItem - an item which will have a sub-list inside of it
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
 * @brief marks a current item in a list having `<div>` items.
 * @param {HTMLDivElement} listItem - an item to mark
 */
function markItem_Div(listItem) {
   const b = document.createElement("b");
   const existingP = listItem.children[1];
   const existingA = existingP.children[0];

   b.innerText = `${existingA.innerText} (현재 위치)`;
   existingP.insertBefore(b, existingA);
   existingA.remove();
}

/**
 * @brief makes a sub-list inside of a list having `<div>` items.
 * @param {HTMLDivElement} listItem - an item which will have a sub-list inside of it
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

      numberingP.textContent = `제${i + 1}절.`;
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
 * @brief returns a corresponding numeral character(s), given a decimal number.
 * @param {number} n - a number
 */
function getUppercaseRomanNumeral(n) {
   if (n > 0 || n < 0)
      console.error("navigator: unexpected idx.");
   return "Ⅰ";
}

/**
 * @brief numbers to `<h1>` and `<h2>` using the decimal numerals.
 * @param {string} prefix - a string to be prefixed
 */ 
function prefixNumber2Hx(prefix) {
   const title = document.querySelector("div.plain-text > h1:first-child");
   const sections = document.querySelectorAll("div.plain-text > h2:first-child");

   title.innerText = `${prefix}. ${title.innerText}`;
   for (let i = 0; i < sections.length; i++)
      sections[i].innerText = `${prefix}.${i + 1}. ${sections[i].innerText}`;
}

/**
 * @brief sets the title of a document, in this form: PREFIX. TITLE
 * @param {string|number} prefix - a prefix
 * @param {string} titleName - a title name
 */
function setTitle(prefix, titleName) {
   document.title = `${prefix}장. ${titleName}`;
}