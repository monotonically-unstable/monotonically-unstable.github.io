---
layout: post
title:  "The two phase name lookup"
date:   2015-07-18 23:46:52
categories: c++_compilation
---

<a name="intro">[איך בונים מכונית? על constructors ולמה הם צריכים להיות ריקים.](#intro)
=======================================================

בניית מכונית היא לא משחק ילדים, אלא אם אתם בונים מכונית מלגו.. אבל גם על זה אפשר להתווכח. הבעיה נעשית מסובכת יותר כשאתם בונים מחלקת מכונית בשפת התכנות האהובה עליכם.

שהתחלתי לכתוב את הפוסט הזה, קצת התקשתי בהגדרה של מכונית. האם למכונית יש התנהגות מסוימת או שמא מכונית היא צריכה להיות Plain Old Data? 
החלטתי להתייחס למכונית כפי שמתייחסים אליה בשיעורי הנהיגה, למכונית יש מספר התנהגויות שיכולות להיות מופעלות ע"י המשתמש.

* הגבר מהירות
* הנמך מהירות
* פנה ימינה
* פנה יותר מדי ימינה
* פנה שמאלה
* תפעיל מזגן

נתבונן במימוש ספציפי של אחת המכוניות:

<pre class="prettyprint"><code style="direction: ltr;">class MercedesCar
{
public:
	MercedesCar ()
	{
		model = "Motorwagen";
		productionYear = 1886;
		wheels = new HotSteelWheels[4];
		radio = new FutureRadio();		
		registerCarInDatabase();
	}

};</code></pre>

