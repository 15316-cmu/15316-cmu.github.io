/**
 * semester-dropdown.js
 *
 * Dynamically populates the "Previous Semesters" dropdown.
 * This script is shared across all semester directories to ensure
 * the navigation menu remains up-to-date without manual copying.
 */

(function () {
  // List of all semesters. 
  // New semesters should be added to the TOP of this list.
  const semesters = [
    { text: "Spring 2026", url: "../2026/index.html" },
    { text: "Fall 2024", url: "../2024/index.html" },
    { text: "Fall 2023", url: "../2023/index.html" },
    { text: "Fall 2022", url: "../2022/index.html" },
    { text: "Fall 2021", url: "../2021/index.html" },
    { text: "Fall 2020", url: "../2020/index.html" },
    { text: "Fall 2019", url: "../2019/index.html" },
    { text: "Spring 2018", url: "../2018/index.html" }
    // Add older semesters here if necessary
  ];

  const dropdownList = document.getElementById('semester-dropdown-list');

  if (dropdownList) {
    semesters.forEach(sem => {
      // Create list item
      const li = document.createElement('li');
      li.className = 'nav-item bg-dark';

      // Create link
      const a = document.createElement('a');
      a.className = 'dropdown-item nav-link bg-dark';
      a.href = sem.url;
      a.textContent = sem.text;

      // Append
      li.appendChild(a);
      dropdownList.appendChild(li);
    });
  } else {
    console.warn("semester-dropdown.js: element with id 'semester-dropdown-list' not found.");
  }
})();
