'use client';
import { useEffect, useState } from 'react';
import Editor from 'react-simple-code-editor';
import Prism from 'prismjs';
import 'prismjs/components/prism-sql';
import 'prismjs/themes/prism-tomorrow.css';
import Table from './components/Table';
import Notification from './components/Notification';

export default function Home() {
  const [data, setData] = useState([]);
  const [query, setQuery] = useState(`create table Datitos from file hospitalesopendata using AVL\n`);
  const [time, setTime] = useState<number | null>(null);
  const [notification, setNotification] = useState<string | null>(null);

  const executeQuery = (queryToExecute: string | undefined) => {
    if (!queryToExecute) return;
    const startTime = performance.now();

    fetch('http://0.0.0.0:18080/query', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: queryToExecute
    })
      .then((res) => res.json())
      .then((data) => {
        if (data["message"]) {
          setNotification(data["message"]);
          setData([]);
        } else {
          console.log(data);
          setData(data);
        }
        const endTime = performance.now();
        setTime(endTime - startTime);
      });
  };

  return (
    <main className="flex flex-col max-w-7xl gap-3 p-20 mx-auto">
      <section className="flex flex-row justify-between w-full rounded-md p-3 border">
        <div className="flex gap-3">
          <p>MyDB.db</p>
        </div>
        <div className="flex">
          <p>v1.0.0</p>
        </div>
      </section>

      <section className="flex flex-col rounded-md p-3 gap-3 border">
        <section className="flex h-32 rounded-md p-2 border">
          <Editor
            value={query}
            onValueChange={code => setQuery(code)}
            highlight={code => Prism.highlight(code, Prism.languages.sql, 'sql')}
            padding={4}
            style={{
              fontFamily: '"Fira code", "Fira Mono", monospace',
              fontSize: 12,
              width: '100%',
            }}
          />
        </section>

        <section className="flex flex-row gap-3 justify-between text-sm rounded-md p-3 border">
          <div className='flex align-middle'>
            {notification && (
              <Notification
                message={notification}
                onClose={() => setNotification(null)}
              />
            )}
          </div>
          <div className='flex flex-row gap-3'>
            <button className="border p-2 rounded-sm bg-gray-50 hover:bg-gray-100"
              onClick={() => executeQuery(window.getSelection()?.toString())}>Execute Selected</button>
            <button className="border p-2 rounded-sm bg-gray-50 hover:bg-gray-100"
              onClick={() => executeQuery(query)}>Execute Query</button>
          </div>
        </section>

        <section className="flex flex-col border rounded-md p-3">
          <Table data={data} time={time} />
        </section>
      </section>

    </main>
  );
}
